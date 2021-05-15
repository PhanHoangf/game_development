#pragma once
#include "GameObject.h"

#define MUSHROOM_BBOX_WIDTH 16
#define MUSHROOM_BBOX_HEIGHT 16

#define MUSHROOM_SPEED 0.15f
#define MUSHROOM_FALL_SPEED 0.015f

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

