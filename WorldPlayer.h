#pragma once
#include "GameObject.h"
#include "Game.h"

#define PLAYER_SPEED	0.1f

#define PLAYER_STATE_IDLE			0
#define PLAYER_STATE_RIGHT			100
#define PLAYER_STATE_LEFT			111
#define PLAYER_STATE_UP				200
#define PLAYER_STATE_DOWN			201

#define PLAYER_BBOX_WIDTH	8	

// SMALL
#define MARIO_ANI_SMALL			0
//BIG
#define MARIO_ANI_BIG			1
//TAIL
#define MARIO_ANI_TAIL			2
//FIRE
#define MARIO_ANI_FIRE			3

#define MARIO	0
#define LUIGI	1

class WorldPlayer :public CGameObject {
	int level;
	
	
public:
	int moveLeft, moveUp, moveRight, moveBottom, sceneId;
	WorldPlayer(float x = 0.0f, float y = 0.0f);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void SetLevel(int l) { level = l; };
	void SetState(int state);
	void ChooseScene(int sceneId) { CGame::GetInstance()->SwitchScene(sceneId); }
};