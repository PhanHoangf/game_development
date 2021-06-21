#pragma once
#include "GameObject.h"

#define FIRE_FLOWER_STATE_IDLE	0
#define FIRE_FLOWER_STATE_UP	1

#define FIRE_FLOWER_SPEED 0.07f
#define FIRE_FLOWER_ANI_ID	38

#define FIRE_FLOWER_BBOX_HEIGHT	16

class FireFlower :public CGameObject {
public:
	bool isAppear = false;
public:
	FireFlower(float x, float y);
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void SetState(int state);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) {
		l = x;
		t = y;
		r = x + FIRE_FLOWER_BBOX_HEIGHT;
		b = y + FIRE_FLOWER_BBOX_HEIGHT;
	};
};