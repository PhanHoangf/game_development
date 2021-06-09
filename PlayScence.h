#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "Koopas.h"
#include "Map.h"
#include "GreenBush.h"
#include "HUD.h"
#include "Grid.h"

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
#define SCENE_TILE_MAP 7
#define SCENE_GRID	8

#define OBJECT_TYPE_MARIO	0
#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_GOOMBA	2
#define OBJECT_TYPE_KOOPAS	3
#define OBJECT_TYPE_BUSH 10
#define OBJECT_INVISIBLE 20
#define OBJECT_QUESTION_BRICK 142
#define OBJECT_TYPE_COINT 6
#define OBJECT_TYPE_BLOCK_LINE 4
#define OBJECT_TYPE_PIRANHA_PLANT 7
#define OBJECT_TYPE_BREAKABLEBRICK		112
#define OBJECT_TYPE_PIRANHA_FIRE_PLANT 70
#define OBJECT_TYPE_GOOMBA_RED 77

#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 1024


class CPlayScene : public CScene
{
protected:
	CMario* player;					// A play scene has to have player, right? 

	vector<LPGAMEOBJECT> objects;  // Static objects
	vector<LPGAMEOBJECT> mObjects; //! Moving objects
	vector<Unit*> objectGrid;
	vector<LPGAMEOBJECT> specialObjects; //! Coin in brick 
	vector<LPGAMEOBJECT> objInPipe;
	CMap* currentMap;
	HUD* hud;

	Grid* grid;
	int objId = 0;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_TILEMAP_DATA(string line);
	void _ParseSection_GRID_DATA(string line);

	void _LoadGridFile(string filePath);
	void UpdateGrid();
	void GetObjectFromGrid();
public:
	CPlayScene(int id, LPCWSTR filePath);
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	virtual void SetCam(float cx, float cy, DWORD dt = 0);
	CMario* GetPlayer() { return player; }
	vector<LPGAMEOBJECT> GetObjects() { return objects; }
	void AddObject(LPGAMEOBJECT obj) { this->objects.push_back(obj); }
	void AddSpecialObject(LPGAMEOBJECT obj) { this->specialObjects.push_back(obj); }
	void AddMovingObject(LPGAMEOBJECT obj) { this->mObjects.push_back(obj); }
	void deleteLastObject() { this->objects.pop_back(); }
	//friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CPlayScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};

