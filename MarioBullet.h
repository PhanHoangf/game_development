#pragma once
#include "GameObject.h"

#define MARIO_BULLET_BBOX_WIDTH		9
#define MARIO_BULLET_BBOX_HEIGHT	9

#define MARIO_BULLET_SPEED_X	0.15f
#define MARIO_BULLET_SPEED_Y	0.005f


#define MARIO_BULLET_ANI_RIGHT		0
#define MARIO_BULLET_ANI_LEFT		1	

#define MARIO_BULLET_ANI_SET_ID		9

#define MARIO_BULLET_STATE_UP		0
#define MARIO_BULLET_STATE_DOWN		1

class MarioBullet :public CGameObject {
public:
	bool isAppear = false;
	float limitY = 0;
public:
	MarioBullet(float nx) : CGameObject() {
		
		this->nx = nx;
	};
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void SetState(int state);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) {
		l = x;
		t = y;
		r = x + MARIO_BULLET_BBOX_WIDTH;
		b = y + MARIO_BULLET_BBOX_HEIGHT;
	};
	void SetLimitY(float y) {
		limitY = y - 25.0f;
	};
};