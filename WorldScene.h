#pragma once

#include "Scence.h"
#include "Map.h"
#include "GameObject.h"
#include "HUD.h"
#include "WorldPlayer.h"
#include "PlayScence.h"

#define SCENE1_X	64
#define SCENE1_Y	16

#define SCENE4_X	160
#define SCENE4_Y	48



class WorldScene : public CScene {
	CMap* currentMap = NULL;
	HUD* hud = NULL;
	vector<LPGAMEOBJECT> objects;
	WorldPlayer* player = NULL;


	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_TILEMAP_DATA(string line);

public:
	WorldScene(int id, LPCWSTR filePath);
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	CMap* GetMap() { return currentMap; }
	HUD* GetHUD() { return hud; }
	WorldPlayer* GetPlayer() { return player; }
};

class WorldSceneKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states) {};
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode) {};
	WorldSceneKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};

