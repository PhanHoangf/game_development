#pragma once

#include "GameObject.h"
#include "Utils.h"
#include "Brick.h"
#include "Block.h"

#define BOOMERANG_BBOX_WIDTH	16
#define BOOMERANG_BBOX_HEIGHT	16

#define BOOMERANG_STATE_IDLE	0
#define BOOMERANG_STATE_THROW	1
#define	BOOMERANG_STATE_COMEBACK	2

#define BOOMERANG_ANI_IDLE_LEFT	1
#define BOOMERANG_ANI_THROW_LEFT 0

#define MAX_BOOMERANG_SPEED 0.15f
#define BOOMERANG_ACCELERATION	0.0005f

#define BOOMERANG_GRAVITY 0.03f

class Boomerang :public CGameObject {
	bool isAppear = false;
	float limitY = 0.0f;
public:
	Boomerang(float x, float y);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	void SetState(int state);

	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) {
		l = x;
		t = y;
		r = x + BOOMERANG_BBOX_WIDTH;
		b = y + BOOMERANG_BBOX_HEIGHT;
	};

	void SetIsAppear(bool ap) { this->isAppear = ap; }
	bool GetIsAppear() { return this->isAppear; }

	void SetLimitBoomerangY(float y) { this->limitY = y - 24; }
};