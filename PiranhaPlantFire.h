#pragma once
#include "PiranhaPlant.h"

#define PIRANHAPLANT_GREEN_TYPE					0
#define PIRANHAPLANT_RED_TYPE					1

#define PIRANHAPLANT_BBOX_WIDTH					20
#define PIRANHAPLANT_GREEN_BBOX_HEIGHT			24
#define PIRANHAPLANT_RED_BBOX_HEIGHT			32
#define PIRANHAPLANT_DARTING_SPEED				0.03f
#define PIRANHAPLANT_BBOX_DEATH_WIDTH			16
#define PIRANHAPLANT_BBOX_DEATH_HEIGHT			24

#define PIRANHAPLANT_STATE_DARTING				0
#define PIRANHAPLANT_STATE_SHOOTING				100
#define PIRANHAPLANT_STATE_DEATH				200
#define PIRANHAPLANT_STATE_INACTIVE				300

#define PIRANHAPLANT_ANI_RIGHT_UP			0
#define PIRANHAPLANT_ANI_RIGHT_DOWN			1
#define PIRANHAPLANT_ANI_LEFT_UP			2
#define PIRANHAPLANT_ANI_LEFT_DOWN			3
#define PIRANHAPLANT_ANI_DEATH				4

#define PIRANHAPLANT_DELAY_TIME			750
#define PIRANHAPLANT_AIM_TIME			750
#define PIRANHAPLANT_DIYING_TIME		250

#define PIRANHAPLANT_ACTIVE_RANGE		20

class PiranhaPlantFire :
	public PiranhaPlant
{
	DWORD shooting_start = 0;
	DWORD aim_start = 0;
	DWORD delay_start = 0;
	DWORD dying_start = 0;
	float limitY = 0;
	bool Up = false;
	bool Right = false;
	int BBHeight = 0;
	
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
public:
	PiranhaPlantFire(int tag = PIRANHAPLANT_GREEN_TYPE);
	void GetDirect();
	void StartShooting() { shooting_start = GetTickCount64(); }
	void StartAim() { aim_start = GetTickCount64(); }
	void StartDelay() { delay_start = GetTickCount64(); }
	void StartDying() { dying_start = GetTickCount64(); }

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void SetState(int state);

	void SetLimitY(float ly)
	{
		if (tag == PIRANHAPLANT_GREEN_TYPE)
			BBHeight = PIRANHAPLANT_GREEN_BBOX_HEIGHT;
		else
			BBHeight = PIRANHAPLANT_RED_BBOX_HEIGHT;
		limitY = ly - BBHeight;
	}
};