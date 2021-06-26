#pragma once
#include "Scence.h"
#include "Goomba.h"
#include "Koopas.h"
#include "Mario.h"
#include "Game.h"
#include <dinput.h>

#define OBJECT_TYPE_MARIO		0
#define OBJECT_TYPE_GROUND		4
#define OBJECT_TYPE_LEAF		5
#define OBJECT_TYPE_GOOMBA		1
#define OBJECT_TYPE_MUSHROOM	2
#define OBJECT_TYPE_KOOPAS		3
#define OBJECT_TYPE_OTHERS		6

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
#define SCENE_TILE_MAP 7
#define SCENE_GRID	8

#define MAX_SCENE_LINE 1024
#define ANISET_BACKGROUND_ID	50
#define ANISET_GROUND_ID	21
#define ANISET_ARROW_ID	51

#define THREE_X	112
#define THREE_Y	96
#define ARROW_Y 144
#define ARROW_X	70

#define GROUND_WIDTH	256

#define SCROLLING_TIME	2000
#define SWITCH_TIME	3000

#define WORLD_SCENE_ID	0

class CIntroScene : public CScene {
public:
	
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);

public:
	/*CGoomba* goomba = nullptr;
	CKoopas* koopas = nullptr;
	CMario* mario = nullptr;
	CMario* luigi = nullptr;
	CMario* player;*/
	vector<LPGAMEOBJECT> objects;
	LPANIMATION_SET BackGround;
	LPANIMATION Three;
	LPANIMATION_SET Arrow;
	DWORD start_switch = 0;
	bool isSwitching = false;
	//DWORD scrolling_up = 0;
	//DWORD scrolling_down = 0;
	//int section;
	//float pointY = 0; //! For scrolling background up
	//int luigiJump = 0;
	CIntroScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	/*void StartScrolling() { scrolling_up = GetTickCount64(); }
	void StartScrollingDown() { scrolling_down = GetTickCount64(); }*/
};

class IntroSceneKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states) {};
	virtual void OnKeyDown(int KeyCode) {
		CIntroScene* intro = ((CIntroScene*)CGame::GetInstance()->GetCurrentScene());
		switch (KeyCode)
		{
		case DIK_RETURN:
			intro->start_switch = GetTickCount64();
			intro->isSwitching = true;
		default:
			break;
		}
	};
	virtual void OnKeyUp(int KeyCode) {};
	IntroSceneKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};