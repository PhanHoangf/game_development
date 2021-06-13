#pragma once
#include "GameObject.h"

#define BOOMERANG_KOOPAS_BBOX_WIDTH 16
#define BOOMERANG_KOOPAS_BBOX_HEIGHT 24

#define BOOMERANG_MAX_THROW 40
#define BOOMERANG_KOOPAS_SPEED 0.03f
#define BOOMERANG_KOOPAS_DIE_REFLECT_SPEED 0.005f
#define BOOMERANG_KOOPAS_GRAVITY 0.002f

#define BOOMERANG_KOOPAS_STATE_WALKING 1
#define BOOMERANG_KOOPAS_STATE_DIE 2
#define BOOMERANG_KOOPAS_STATE_IDLE 0

#define BOOMERANG_KOOPAS_ANI_RIGHT	1
#define BOOMERANG_KOOPAS_ANI_LEFT	0

#define LIMIT_X 397.0f

#define IDLE_TIME 200

class BoomerangKoopas : public CGameObject {
	bool isReadyToThrow = false;
	bool isThrowingBoomerang = false;

	bool isBoomerangComback = false;
	float limit_x = 0.0f;

	DWORD start_idle = 0;
	bool isIdle = false;
public:
	BoomerangKoopas(float x, float y);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();

	void SetState(int state);

	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) {
		l = x;
		t = y;
		r = x + BOOMERANG_KOOPAS_BBOX_WIDTH;
		b = y + BOOMERANG_KOOPAS_BBOX_HEIGHT;
	};

	void StartIdle() { start_idle = GetTickCount64(); isIdle = true; }
	void StopIdle() { start_idle = 0; isIdle = false; }
};