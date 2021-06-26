#pragma once
#include "GameObject.h"

#define MUSHROOM_BBOX_WIDTH 16
#define MUSHROOM_BBOX_HEIGHT 16

#define MUSHROOM_SPEED 0.07f
#define MUSHROOM_FALL_SPEED 0.015f

#define MUSHROOM_STATE_UP	6
#define MUSHROOM_STATE_LEFT	-1
#define MUSHROOM_STATE_RIGHT 1
#define MUSHROOM_STATE_FALL	5
#define MUSHROOM_STATE_IDLE	0

#define MUSHROOM_GRAVITY 0.2f

class MushRoom:public CGameObject
{
	bool isAppear;
public:
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) {
		l = x;
		t = y;
		r = x + MUSHROOM_BBOX_WIDTH;
		b = y + MUSHROOM_BBOX_HEIGHT;
	};
	void SetState(int state);
	void SetAppear(bool appear) {
		isAppear = appear;
	}
	bool GetAppear() { return isAppear; }
	MushRoom();
	~MushRoom();
};

