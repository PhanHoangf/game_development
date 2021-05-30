#pragma once
#include "GameObject.h"
#include "Utils.h"
#define PIRANHAPLANT_BBOX_WIDTH					20
#define PIRANHAPLANT_BBOX_HEIGHT				24
#define PIRANHAPLANT_DARTING_SPEED				0.03f

#define PIRANHAPLANT_STATE_DARTING				0
#define PIRANHAPLANT_STATE_BITING				100
#define PIRANHAPLANT_STATE_DEATH				200
#define PIRANHAPLANT_STATE_INACTIVE				300

#define PIRANHAPLANT_ACTIVE_RANGE			20

#define PIRANHAPLANT_ANI_DARTING			0
#define PIRANHAPLANT_ANI_DEATH				1

#define PIRANHAPLANT_BITING_TIME		1000
#define PIRANHAPLANT_DIYING_TIME		250

class PiranhaPlant :
	public CGameObject
{
	DWORD biting_start = 0;
	DWORD dying_start = 0;
	float limitY = 0;
public:
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void SetState(int state);
	void SetLimitY(float ly) { limitY = ly - PIRANHAPLANT_BBOX_HEIGHT; DebugOut(L"LimitY::%f\n", limitY); }
	void StartBitting() { biting_start = GetTickCount(); }
	void StartDying() { dying_start = GetTickCount(); }
	PiranhaPlant();
};

