#pragma once
#include "GameObject.h"

#define LEAF_BBOX_WIDTH  16
#define LEAF_BBOX_HEIGHT 16

#define LEAF_UP_HEIGHT	 48

#define LEAF_ANI_SET_ID		36

#define LEAF_ANI_RIGHT		0
#define LEAF_ANI_LEFT		1

#define LEAF_GRAVITY		0.025f
#define LEAF_SPEED			0.075f
#define LEAF_SPEED_UP		0.05f

#define LEAF_STATE_IDLE		0
#define LEAF_STATE_UP		100
#define LEAF_STATE_FALLING	111

#define LEAF_FALLING_TIME	500

class Leaf :
    public CGameObject
{
	float start_y;
	DWORD start_timing;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
public:
	bool isAppear = false;
	void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void SetAppear(bool isAppear)
	{
		this->isAppear = isAppear;
	}
	void StartTiming() { start_timing = GetTickCount64(); }
};

