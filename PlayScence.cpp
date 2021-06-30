#include <iostream>
#include <fstream>

#include "PlayScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "StopPoint.h"
#include "QuestionBrick.h"
#include "Coin.h"
#include "Block.h"
#include "MushRoom.h"
#include "PiranhaPlant.h"
#include "BreakableBrick.h"
#include "PiranhaPlantFire.h"
#include "RedGoomba.h"
#include "Leaf.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
	//this->grid = new Grid();
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/




void CPlayScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CPlayScene::_ParseSection_SPRITES(string line)
{
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

	DebugOut(L"[ID] %d \n", ID);

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
	DebugOut(L"[SUCCESS] SPRITE %d ADDED!\n", ID);
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
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
}

void CPlayScene::_ParseSection_TILEMAP_DATA(string line)
{
	int ID, rowMap, columnMap, columnTile, rowTile, totalTiles;
	LPCWSTR path = ToLPCWSTR(line);
	ifstream f;

	//const int testRow = rowMap;
	//const int testColumn = columnMap;

	f.open(path);
	f >> ID >> rowMap >> columnMap >> rowTile >> columnTile >> totalTiles;
	////Init Map Matrix

	//int** TileMapData;
	int** TileMapData = new int* [rowMap];
	for (int i = 0; i < rowMap; i++)
	{
		TileMapData[i] = new int[columnMap];
		for (int j = 0; j < columnMap; j++)
			f >> TileMapData[i][j];
	}
	f.close();

	currentMap = new CMap(ID, rowMap, columnMap, rowTile, columnTile, totalTiles);
	currentMap->ExtractTileFromTileSet();
	currentMap->SetTileMapData(TileMapData);
}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

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
}

/*
	Parse a line in section [OBJECTS]
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	DebugOut(L"--> %s\n", ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y
	int objID = 0, objRow = 0, objCol = 0;
	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	int  tag = 0, option_tag_1 = 0, option_tag_2 = 0, option_tag_3 = 0;
	if (tokens.size() >= 5)
		tag = atof(tokens[4].c_str());
	if (tokens.size() >= 6)
		option_tag_1 = atof(tokens[5].c_str());
	if (tokens.size() >= 7)
		option_tag_2 = atof(tokens[6].c_str());
	if (tokens.size() >= 8)
		option_tag_3 = atof(tokens[7].c_str());
	if (tokens.size() >= 9)
		objID = atof(tokens[8].c_str());
	if (tokens.size() >= 10)
		objRow = atof(tokens[9].c_str());
	if (tokens.size() >= 11)
		objCol = atof(tokens[10].c_str());

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario(x, y);
		player = (CMario*)obj;
		DebugOut(L"[INFO] Player object created!\n");
		break;
	case OBJECT_TYPE_GOOMBA:
		obj = new CGoomba();
		break;
	case OBJECT_TYPE_GOOMBA_RED:
		obj = new RedGoomba();
		break;
	case OBJECT_TYPE_BRICK:
		obj = new CBrick(x, y, option_tag_1, option_tag_2, option_tag_3);
		break;
	case OBJECT_TYPE_COINT: obj = new Coin(); break;
	case OBJECT_QUESTION_BRICK:
		obj = new QuestionBrick(option_tag_1, option_tag_2);
		break;
	case OBJECT_TYPE_BREAKABLEBRICK:
		obj = new BreakableBrick(x, y, option_tag_1, option_tag_2, option_tag_3);
		break;
	case OBJECT_TYPE_KOOPAS:
		obj = new CKoopas(tag);
		obj->SetTag(tag);
		((CKoopas*)obj)->start_tag = tag;
		//obj->SetType(MOVING);
		((CKoopas*)obj)->start_x = x;
		((CKoopas*)obj)->start_y = y;
		break;
	case OBJECT_TYPE_PIRANHA_PLANT:
		obj = new PiranhaPlant();
		((PiranhaPlant*)obj)->SetLimitY(y);
		break;
	case OBJECT_TYPE_PIRANHA_FIRE_PLANT:
		obj = new PiranhaPlantFire(tag);
		((PiranhaPlantFire*)obj)->SetLimitY(y);
		break;
	case OBJECT_INVISIBLE:
		obj = new StopPoint();
		break;
	case OBJECT_TYPE_BLOCK_LINE:
		obj = new Block();
		break;
	case OBJECT_TYPE_PORTAL:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		int scene_id = atoi(tokens[6].c_str());
		obj = new CPortal(scene_id, x, y);
		break;
	}
	case OBJECT_TYPE_CARD: {
		obj = new CardItem();
		break;
	}
	case OBJECT_TYPE_BOOMERANG_KOOPAS:
		obj = new BoomerangKoopas(x, y);
		break;
	case OBJECT_TYPE_MUSICAL_BRICK: {
		obj = new MusicBrick(x, y);
		break;
	}
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);

	/*if (dynamic_cast<MushRoom*>(obj) || dynamic_cast<PiranhaPlant*>(obj)) {
		mObjects.push_back(obj);
	}
	else objects.push_back(obj);*/

	if (dynamic_cast<CMario*>(obj)) {
		return;
	}
	//else if (dynamic_cast<CardItem*>(obj)) {
	//	this->AddSpecialObject(obj);
	//}
	if (dynamic_cast<CPortal*>(obj)) {
		this->specialObjects.push_back(obj);
	}
	else {
		obj->id = objID;
		Unit* unit = new Unit(this->grid, obj, objRow, objCol);
	}
}

void CPlayScene::Load()
{
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
		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line == "[TILE_MAP]") {
			section = SCENE_TILE_MAP; continue;
		}
		if (line == "[GRID]") {
			section = SCENE_GRID; continue;
		}
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
		case SCENE_TILE_MAP: _ParseSection_TILEMAP_DATA(line); break;
		case SCENE_GRID: _LoadGridFile(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	hud = new HUD();

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CPlayScene::_LoadGridFile(string filePath) {
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", filePath);

	ifstream f;
	f.open(filePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;
	DebugOut(L"%d", section);
	char str[MAX_SCENE_LINE];

	while (f.getline(str, MAX_SCENE_LINE)) {
		string line(str);

		if (line[0] == '#') continue;

		_ParseSection_GRID_DATA(line);
	}
	f.close();
}

void CPlayScene::_ParseSection_GRID_DATA(string line) {
	vector<string> tokens = split(line);

	DebugOut(L"--> %s\n", ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least row, col, objId, type

	int cols = atoi(tokens[0].c_str());
	int rows = atoi(tokens[1].c_str());
	int mapWidth = atoi(tokens[2].c_str());
	int mapHeight = atoi(tokens[3].c_str());

	this->grid = new Grid(rows, cols, mapWidth, mapHeight);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 
	CGame* game = CGame::GetInstance();

	float cam_x, cam_y;

	cam_x = game->GetCamPosX();
	cam_y = game->GetCamPosY();

	vector<LPGAMEOBJECT> coObjects;
	coObjects.clear();

	GetObjectFromGrid();




	for (size_t i = 0;i < objRenderFirst.size(); i++) {
		coObjects.push_back(objRenderFirst[i]);
	}

	for (size_t i = 0;i < objRenderSecond.size(); i++) {
		coObjects.push_back(objRenderSecond[i]);
	}

	for (size_t i = 0;i < specialObjects.size(); i++) {
		if (!specialObjects[i]->GetIsDestroy() && !specialObjects[i]->isIgnore)
			coObjects.push_back(specialObjects[i]);
	}
	//for (size_t i = 0; i < objects.size(); i++)
	//{
	//	if (!objects[i]->GetIsDestroy())
	//		coObjects.push_back(objects[i]);
	//}

	//for (size_t i = 0; i < mObjects.size(); i++)
	//{
	//	if (!objects[i]->GetIsDestroy())
	//		coObjects.push_back(mObjects[i]);
	//}

	player->Update(dt, &coObjects);

	//coObjects.push_back(player);
	//for (size_t i = 0; i < objectGrid.size(); i++)
	//{
	//	objectGrid[i]->GetObject()->Update(dt, &coObjects);
	//}

	for (size_t i = 0; i < objRenderFirst.size(); i++)
	{
		if (objRenderFirst[i]->IsInViewPort()) {
			objRenderFirst[i]->Update(dt, &coObjects);
		}
		else objRenderFirst[i]->Update(0, &coObjects);
	}

	for (size_t i = 0; i < objRenderSecond.size(); i++)
	{
		if (objRenderSecond[i]->IsInViewPort() && !objRenderSecond[i]->isIgnore) {
			objRenderSecond[i]->Update(dt, &coObjects);
		}
		else {
			objRenderSecond[i]->Update(0, &coObjects);
		}
	}

	for (size_t i = 0;i < specialObjects.size(); i++) {
		if (specialObjects[i]->IsInViewPort() && !specialObjects[i]->isDestroyed) {
			specialObjects[i]->Update(dt, &coObjects);
		}
		else {
			specialObjects[i]->Update(0, &coObjects);
		}
	}

	//for (size_t i = 0; i < mObjects.size(); i++) {
	//	mObjects[i]->Update(dt, &coObjects);
	//}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;
	SetCam(player->x, player->y, dt);
	hud->Update(dt, &coObjects);
	UpdateGrid();
}

void CPlayScene::GetObjectFromGrid() {
	CGame* game = CGame::GetInstance();

	float cam_x, cam_y;

	cam_x = game->GetCamPosX();
	cam_y = game->GetCamPosY();

	objectGrid.clear();
	objRenderFirst.clear();
	objRenderSecond.clear();

	objectGrid = grid->getObjectsInViewPort(cam_x, cam_y);

	for (size_t i = 0; i < objectGrid.size(); i++)
	{
		LPGAMEOBJECT obj = objectGrid[i]->GetObject();
		if (dynamic_cast<PiranhaPlant*>(obj) || dynamic_cast<PiranhaPlantFire*>(obj)) {
			objRenderFirst.push_back(obj);
		}
		else if (dynamic_cast<CKoopas*>(obj)) {
			CKoopas* kp = dynamic_cast<CKoopas*>(obj);
			if (kp->isReviable && !kp->IsInViewPort()) {
				kp->Reset();
				kp->isReviable = false;
				objRenderSecond.push_back(kp);
			}
			else objRenderSecond.push_back(kp);
		}
		else objRenderSecond.push_back(obj);
	}
}

void CPlayScene::UpdateGrid() {
	for (unsigned int i = 0; i < objectGrid.size(); i++)
	{
		LPGAMEOBJECT obj = objectGrid[i]->GetObject();
		float newPosX, newPosY;
		obj->GetPosition(newPosX, newPosY);
		objectGrid[i]->Move(newPosX, newPosY);
	}
}

void CPlayScene::Render()
{
	currentMap->DrawMap();

	/*for (int i = 0; i < mObjects.size(); i++)
	{
		if (!mObjects[i]->isDestroyed) {
			mObjects[i]->Render();
		}
	}*/





	for (int i = 0; i < objRenderFirst.size(); i++)
	{
		objRenderFirst[i]->Render();
	}

	for (int i = 0; i < objRenderSecond.size(); i++)
	{
		if (!objRenderSecond[i]->isIgnore)
			objRenderSecond[i]->Render();
	}

	for (int i = 0; i < specialObjects.size(); i++)
	{
		if (!specialObjects[i]->isDestroyed) {
			specialObjects[i]->Render();
		}
	}

	player->Render();

	hud->Render();
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	CBackupHud* buh = CBackupHud::GetInstance();
	for (int i = 0; i < specialObjects.size(); i++)
		delete specialObjects[i];

	for (int i = 0; i < objRenderFirst.size(); i++)
		delete objRenderFirst[i];

	for (int i = 0; i < objRenderSecond.size(); i++)
		delete objRenderSecond[i];


	objRenderFirst.clear();
	objRenderSecond.clear();
	specialObjects.clear();
	player = NULL;
	grid = NULL;
	if (hud != NULL)
		buh->BackUp(hud->GetMarioLife(), hud->GetScore(), hud->GetMoney(), hud->GetCards());

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	CMario* mario = ((CPlayScene*)scence)->GetPlayer();
	if (mario->GetState() == MARIO_STATE_DIE) return;
	switch (KeyCode)
	{
	case DIK_SPACE:
		if (mario->isTailFlying) {
			mario->vy = -0.025f;
			mario->ay = -MARIO_GRAVITY;
		}
		else {
			mario->SetState(MARIO_STATE_JUMP);
		}
		break;
	case DIK_R:
		mario->Reset();
		break;
	case DIK_T:
		mario->SetState(MARIO_STATE_TRANSFORM);
		mario->StartTransform(MARIO_LEVEL_BIG);
		break;
	case DIK_A:
		mario->SetIsReadyToRun(true);
		mario->SetIsReadyToHold(true);
		break;
	case DIK_S:
		if (mario->GetLevel() == MARIO_LEVEL_TAIL)
			mario->isFlapping = true;
		break;
	case DIK_Q:
		if (mario->GetLevel() == MARIO_LEVEL_TAIL && mario->GetState() != MARIO_STATE_SITDOWN) mario->SetState(MARIO_STATE_TAIL_ATTACK);
		if (mario->GetLevel() == MARIO_LEVEL_FIRE && !mario->GetIsHolding()) mario->ShootFireBall();
		break;
	}
}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode) {
	CMario* mario = ((CPlayScene*)scence)->GetPlayer();
	if (mario->GetState() == MARIO_STATE_DIE) return;
	switch (KeyCode)
	{
	case DIK_SPACE:
		mario->ay = MARIO_GRAVITY;
		break;
	case DIK_A:
		mario->SetIsReadyToRun(false);
		mario->SetIsReadyToHold(false);
		if (mario->GetIsHolding()) {
			mario->SetHolding(false);
		}
		break;
	case DIK_S:
		mario->isFlapping = false;
		break;
	}
}

void CPlayScene::SetCam(float cx, float cy, DWORD dt) {
	float sw, sh, mw, mh, mx, my;
	CGame* game = CGame::GetInstance();
	sw = game->GetScreenWidth();
	sh = game->GetScreenHeight() - 32;
	mw = currentMap->GetMapWidth();
	mh = currentMap->GetMapHeight();
	//DebugOut(L"dt %u\n", dt);
	//Update camera to follow mario
	//if (id == 4)
	//{
	//	sum_dt += dt;
	//	// CamX
	//	cx = game->GetCamPosX();
	//	if (sum_dt >= CAM_CHANGE_TIME)
	//	{
	//		sum_dt = 0;
	//		cx++;
	//	}
	//	if (cx <= 0)//Left Edge
	//		cx = 0;
	//	if (cx >= mw - sw)//Right Edge
	//		cx = mw - sw;

	//	cy = mh - sh;
	//	game->SetCamPos(cx, ceil(cy));
	//	current_map->SetCamPos(cx, ceil(cy));
	//	hud->SetPosition(cx, ceil(cy + sh - HUD_HEIGHT));
	//}
	//hud->SetPosition(cx, ceil(cy + sh - 32));
	/*else
	{*/
	cx -= sw / 2;
	// CamX
	if (cx <= 0)//Left Edge
		cx = 0;
	if (cx >= mw - sw)//Right Edge
		cx = mw - sw;



	//CamY
	if (isTurnOnCamY)
		cy -= sh / 2;
	else
		//cy -= sh / 2;
		cy = mh - sh;

	if (cy <= 0)//Top Edge
		cy = 0;
	if (cy + sh >= mh)//Bottom Edge
		cy = mh - sh;

	//Update CamY when Flying
	if (player->isFlying || player->isTailFlying)
		isTurnOnCamY = true;
	else if (cy > mh - sh - 16)
		isTurnOnCamY = false;

	DebugOut(L"cy::%f\n", cy);
	/*DebugOut(L"mh::%f\n", mh);
	DebugOut(L"sh::%f\n", sh);*/

	game->SetCamPos(ceil(cx), ceil(cy));
	currentMap->SetCamPos(cx, cy);
	hud->SetPosition(ceil(cx), ceil(cy + sh));
	//}
}

void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	CMario* mario = ((CPlayScene*)scence)->GetPlayer();
	if (mario == NULL) return;
	// disable control key when Mario die 
	if (mario->GetState() == MARIO_STATE_DIE) return;
	if (game->IsKeyDown(DIK_RIGHT)) {
		mario->SetState(MARIO_STATE_WALKING_RIGHT);
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		mario->SetState(MARIO_STATE_WALKING_LEFT);
	}
	else if (game->IsKeyDown(DIK_X)) {
		mario->SetState(MARIO_STATE_JUMP_X);
	}
	else if (game->IsKeyDown(DIK_DOWN)) {
		if (mario->GetLevel() == MARIO_LEVEL_BIG && mario->GetIsOnGround() ||
			mario->GetLevel() == MARIO_LEVEL_TAIL && mario->GetIsOnGround() ||
			mario->GetLevel() == MARIO_LEVEL_FIRE && mario->GetIsOnGround())
			mario->SetState(MARIO_STATE_SITDOWN);
	}
	/*else if (game->IsKeyDown(DIK_Q)) {
		if (mario->GetLevel() == MARIO_LEVEL_TAIL) {
			mario->SetState(MARIO_STATE_TAIL_ATTACK);
		}
	}*/
	//else if (game->IsKeyDown(DIK_S)) {
	//	if (mario->GetLevel() == MARIO_LEVEL_TAIL)
	//		mario->isFlapping = true;
	//}
	/*else if (game->IsKeyDown(DIK_A)) {
		mario->SetIsReadyToRun(true);
	}*/
	else
	{
		//&& !mario->isTuring
		if (mario->GetIsOnGround() && !mario->isTuring)
			mario->SetState(MARIO_STATE_IDLE);
	}
}

