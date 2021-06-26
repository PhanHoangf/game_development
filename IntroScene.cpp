#include <iostream>
#include <fstream>

#include "IntroScene.h"
#include "Textures.h"
#include "Utils.h"
//#include "Brick.h"
#include "IntroGround.h"


using namespace std;

CIntroScene::CIntroScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new IntroSceneKeyHandler(this);
	StartScrolling();
}

void CIntroScene::_ParseSection_TEXTURES(string line) {
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CIntroScene::_ParseSection_SPRITES(string line) {
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CIntroScene::_ParseSection_ANIMATIONS(string line) {
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
	if (ani_id == 800)
		Three = ani;
}

void CIntroScene::_ParseSection_ANIMATION_SETS(string line) {
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id
	//DebugOut(L"--> %s\n", ToWSTR(line).c_str());
	int ani_set_id = atoi(tokens[0].c_str());
	LPANIMATION_SET s;
	if (CAnimationSets::GetInstance()->animation_sets[ani_set_id] != NULL)
		s = CAnimationSets::GetInstance()->animation_sets[ani_set_id];
	else
		s = new CAnimationSet();
	CAnimations* animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}
	CAnimationSets::GetInstance()->Add(ani_set_id, s);
	if (ani_set_id == ANISET_BACKGROUND_ID)
		BackGround = s;
}

void CIntroScene::_ParseSection_OBJECTS(string line) {
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y
	int tag = 0, option_tag_1 = 0, option_tag_2 = 0;
	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());
	if (tokens.size() >= 5)
		tag = atof(tokens[4].c_str());
	if (tokens.size() >= 6)
		option_tag_1 = atof(tokens[5].c_str());
	if (tokens.size() >= 7)
		option_tag_2 = atof(tokens[6].c_str());


	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	CGameObject* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_GROUND:
		obj = new IntroGround();
		break;
	case OBJECT_TYPE_MARIO:
		obj = new CMario(x, y);
		if (luigi == NULL) {
			luigi = (CMario*)obj;
			luigi->SetState(MARIO_STATE_IDLE);
			luigi->SetLevel(MARIO_LEVEL_BIG);
		}
		else {
			mario = (CMario*)obj;
			mario->SetDirection(-1);
			mario->SetState(MARIO_STATE_IDLE);
			mario->SetLevel(MARIO_LEVEL_BIG);
			player = mario;
		}
		break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	obj->SetPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
	obj->SetAnimationSet(ani_set);


	objects.push_back(obj);
}

void CIntroScene::Update(DWORD dt) {
	vector<LPGAMEOBJECT> coObjects;

	for (size_t i = 0;i < objects.size(); i++) {
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0;i < objects.size(); i++) {
		objects[i]->Update(dt, &coObjects);
	}


	if (section == 0) {
		mario->SetPosition(mario->start_x, mario->y);
		luigi->SetPosition(luigi->start_x, luigi->y);

		if (GetTickCount64() - scrolling_up >= SCROLLING_TIME / 2)
		{
			mario->SetState(MARIO_STATE_WALKING_LEFT);
			luigi->SetState(MARIO_STATE_WALKING_RIGHT);
		}

		pointY = -(float)(GetTickCount64() - scrolling_up) / 10;
		if (GetTickCount64() - scrolling_up >= SCROLLING_TIME)
			StartFirstSection();
	}
	if (section == 1) {
		if (luigi->x >= 48.0f && luigi->state != MARIO_STATE_JUMP && luigiJump == 0)
		{
			luigi->SetState(MARIO_STATE_JUMP);
			luigi->SetSpeed(0, -MARIO_JUMP_SPEED_MIN);
			luigiJump++;
		}
		else if(luigi->GetIsOnGround()) {
			luigi->SetState(MARIO_STATE_WALKING_RIGHT);
			//luigi->SetSpeed(0, -MARIO_JUMP_SPEED_MIN);
		}
		if (luigi->y <= MARIO_BIG_BBOX_HEIGHT)
		{
			luigi->y = MARIO_BIG_BBOX_HEIGHT;
			//luigi->isDeflect = false;

			mario->SetState(MARIO_STATE_IDLE);
			scrolling_down = GetTickCount64();
			pointY = -187;
		}
		if (scrolling_down != 0)
		{
			if (pointY < 0)
				pointY += 5;
			else
				pointY = 0;
		}
		else
		{
			pointY = -300;
		}
		if (luigi->x > GROUND_WIDTH) {
			scrolling_down = 0;
			StartSecondSection();
			luigi->SetState(MARIO_STATE_IDLE);
			luigi->SetDirection(-1);
			luigi->SetPosition(luigi->x + KOOPAS_BBOX_WIDTH, luigi->y);
			luigi->SetSpeed(0, 0);
		}
	}
}

void CIntroScene::Load() {
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;
	DebugOut(L"%d", section);
	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") { section = SCENE_SECTION_SPRITES; continue; }
		if (line == "[ANIMATIONS]") { section = SCENE_SECTION_ANIMATIONS; continue; }
		if (line == "[ANIMATION_SETS]") { section = SCENE_SECTION_ANIMATION_SETS; continue; }
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; }
		if (line[0] == '[' || line == "") { section = SCENE_SECTION_UNKNOWN; continue; }
		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CIntroScene::Render() {
	BackGround->at(section)->Render(0, pointY);
	if (section >= 2) {
		Three->Render(THREE_X, THREE_Y);
	}
	for (size_t i = 2; i < objects.size(); i++)
		objects[i]->Render();
	if (GetTickCount64() - scrolling_up >= SCROLLING_TIME / 2) {
		mario->Render();
		luigi->Render();
	}
}

void CIntroScene::Unload() {

}

void CIntroScene::StartSecondSection()
{
	section = 2;
	pointY = 0;
	/*for (size_t i = 0; i < fallingObjects.size(); i++)
	{
		fallingObjects[i]->SetSpeed(0, MARIO_JUMP_SPEED_MAX);
		if (dynamic_cast<CMushRoom*>(fallingObjects[i]))
			fallingObjects[i]->SetState(MUSHROOM_STATE_WALK);
		if (dynamic_cast<CLeaf*>(fallingObjects[i]))
		{
			fallingObjects[i]->SetState(LEAF_STATE_FALLING);
			fallingObjects[i]->SetSpeed(LEAF_SPEED, LEAF_GRAVITY * 2);
		}
	}*/
}