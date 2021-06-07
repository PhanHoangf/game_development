#pragma once
#include <iostream>
#include <fstream>
#include "WorldScene.h"
#include "StopPoint.h"


#define OBJECT_TYPE_PLAYER				0
#define OBJECT_TYPE_BRICK				1
#define OBJECT_TYPE_STOP				2
#define OBJECT_TYPE_PORTAL				3
#define OBJECT_TYPE_BUSH				4
#define OBJECT_TYPE_HAMMER				5

using namespace std;

WorldScene::WorldScene(int id, LPCWSTR filePath) :CScene(id, filePath) {
	key_handler = new WorldSceneKeyHandler(this);
}

void WorldScene::_ParseSection_TEXTURES(string line)
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

void WorldScene::_ParseSection_SPRITES(string line)
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

void WorldScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void WorldScene::_ParseSection_ANIMATION_SETS(string line)
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

void WorldScene::_ParseSection_TILEMAP_DATA(string line)
{
	int ID, rowMap, columnMap, columnTile, rowTile, totalTiles;
	LPCWSTR path = ToLPCWSTR(line);
	ifstream f;

	f.open(path);
	f >> ID >> rowMap >> columnMap >> rowTile >> columnTile >> totalTiles;
	//Init Map Matrix

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

void WorldScene::_ParseSection_OBJECTS(string line) {
	vector<string> tokens = split(line);
	int moveLeft, moveUp, moveRight, moveBottom;

	DebugOut(L"--> %s\n", ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	int  tag = 0, option_tag_1 = 0, option_tag_2 = 0;
	if (tokens.size() >= 5)
		moveLeft = atof(tokens[4].c_str());
	if (tokens.size() >= 6)
		moveUp = atof(tokens[5].c_str());
	if (tokens.size() >= 7)
		moveRight = atof(tokens[6].c_str());
	if (tokens.size() >= 8)
		moveBottom = atof(tokens[7].c_str());

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* obj = NULL;
	StopPoint* stp = NULL;
	switch (object_type)
	{
	case OBJECT_TYPE_PLAYER:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] PLAYER object was created before!\n");
			return;
		}
		obj = new WorldPlayer(x, y);
		player = (WorldPlayer*)obj;
		DebugOut(L"[INFO] Player object created!\n");
		break;
	case OBJECT_TYPE_BUSH:
		obj = new GreenBush();
		break;
	case OBJECT_TYPE_STOP:
		obj = new StopPoint();
		stp = (StopPoint*)obj;
		//stp->SetAcceptMovement(moveLeft, moveUp, moveRight, moveBottom);
		break;
	case OBJECT_TYPE_PORTAL:
		obj = new StopPoint();
		stp = (StopPoint*)obj;
		//stp->SetAcceptMovement(moveLeft, moveUp, moveRight, moveBottom);
		break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);
	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);
}

void WorldScene::Load()
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
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	hud = new HUD();

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void WorldScene::Render()
{
	currentMap->DrawMap();
	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();
	hud->Render();
}

void WorldSceneKeyHandler::OnKeyDown(int KeyCode)
{
	WorldPlayer* player = ((WorldScene*)scence)->GetPlayer();
	if (player != NULL)
	{
		switch (KeyCode)
		{
			case DIK_RIGHT:
				//if (player->cgRight)
					player->SetState(PLAYER_STATE_RIGHT);
				break;
			case DIK_LEFT:
				//if (player->cgLeft)
					player->SetState(PLAYER_STATE_LEFT);
				break;
			case DIK_UP:
				//if (player->cgUp)
					player->SetState(PLAYER_STATE_UP);
				break;
			case DIK_DOWN:
				//if (player->cgDown)
					player->SetState(PLAYER_STATE_DOWN);
				break;
			/*case DIK_S:
				if (player->sceneId > 0)
					player->ChooseScene();
				break;*/
		}
	}
}

void WorldScene::Update(DWORD dt) {
	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		if (!objects[i]->GetIsDestroy())
			coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;
	CGame::GetInstance()->SetCamPos(0, 0);
	hud->SetPosition(0, currentMap->GetMapHeight());
}

void WorldScene::Unload() {
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;
	hud = NULL;
	currentMap = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}