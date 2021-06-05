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


class CPlayScene : public CScene
{
protected:
	CMario* player;					// A play scene has to have player, right? 

	vector<LPGAMEOBJECT> objects;  // Static objects
	vector<LPGAMEOBJECT> mObjects; //! Moving objects

	CMap* currentMap;
	HUD* hud;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_TILEMAP_DATA(string line);

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

