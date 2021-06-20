#pragma once

#include "GameObject.h"

#define KOOPAS_WALKING_SPEED			0.03f
#define KOOPAS_PARA_WALKING_SPEED		0.035f
#define KOOPAS_JUMP_SPEED				0.35f
#define KOOPAS_RED_SPEED				0.03f
#define KOOPAS_GRAVITY					0.002f
#define KOOPAS_PARA_GRAVITY				0.001f
#define KOOPAS_DIE_DEFLECT_SPEED		0.3f
#define KOOPAS_SHELL_DEFLECT_SPEED		0.4f
#define KOOPAS_BBOX_WIDTH				16
#define KOOPAS_BBOX_HEIGHT				26
#define KOOPAS_BBOX_SHELL_HEIGHT		16

#define KOOPAS_STATE_WALKING	100
#define KOOPAS_STATE_SHELL_UP	200
#define KOOPAS_STATE_SPINNING	300
#define KOOPAS_STATE_IN_SHELL	400
#define KOOPAS_STATE_DEATH		500
#define KOOPAS_STATE_INACTIVE	600

#define KOOPAS_REVIVE_TIME		2000
#define KOOPAS_SHELL_TIME		5000
#define KOOPAS_DIYING_TIME		1000

#define KOOPAS_ANI_WALKING_RIGHT	0
#define KOOPAS_ANI_SPIN_RIGHT		1
#define KOOPAS_ANI_WALKING_LEFT		2
#define KOOPAS_ANI_SPIN_LEFT		3
#define KOOPAS_ANI_SHELL			4
#define KOOPAS_ANI_SHAKE			5
#define KOOPAS_ANI_SHELL_UP			6
#define KOOPAS_ANI_PARA_RIGHT		7
#define KOOPAS_ANI_PARA_LEFT		8
#define KOOPAS_ANI_SHAKE_UP			9

#define KOOPAS_GREEN		0
#define KOOPAS_RED			1
#define KOOPAS_GREEN_PARA	2
#define KOOPAS_RED_PARA		3

#define KOOPAS_TURN_DIFF	4

#define LIMIT_KOOPAS_BLOCK_X_RIGHT 596
#define LIMIT_KOOPAS_BLOCK_X_LEFT 510

class CKoopas : public CGameObject
{
	DWORD reviving_start = 0;
	DWORD shell_start = 0;
	DWORD dying_start = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
public:
	bool isBeingHeld = false;
	bool isKillable = false;
	float lastStanding_Y;
	bool CanPullBack = false;
	float start_x = 0, start_y = 0;
	int start_tag = 0;
	bool isHaveWings = false;
	bool isReviable = false;
	void StartReviving() { reviving_start = GetTickCount64(); }
	void StartShell() { shell_start = GetTickCount64(); reviving_start = 0; }
	CKoopas(int tag);
	void SetIsBeingHeld(bool m) { isBeingHeld = m; };
	virtual void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void StartDying() { dying_start = GetTickCount64(); }
	void Reset();
	bool CalTurnable(LPGAMEOBJECT object, vector<LPGAMEOBJECT>* coObjects);
	void HandleBeingHeld(LPGAMEOBJECT player);
};