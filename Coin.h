#pragma once
#include "GameObject.h"

#define COIN_BBOX_WIDTH  16
#define COIN_BBOX_HEIGHT 16


#define COIN_ANI_SET_ID		6

#define COIN_STATE_IDLE		0
#define COIN_STATE_UP		100
#define COIN_STATE_DOWN		111

#define COIN_TYPE_NORMAL	6
#define COIN_TYPE_INBRICK	61
#define COIN_TYPE_TRANSFORM	60

#define COIN_FALLING_TIME			300
#define COIN_BEING_PUSHEDUP_TIME	300
#define COIN_EXIST_TIME				10000


class Coin :public CGameObject {
	DWORD timing_start = 0;
	DWORD exist_start = 0;
	bool isAppear;
	int coinPoint;
public:
	void SetAppear(bool appear) { isAppear = appear; }
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void SetState(int state);
	bool GetAppear() { return isAppear; }
	void StartTiming()
	{
		timing_start = GetTickCount64();
	}
	void StartExist()
	{
		exist_start = GetTickCount64();
	}
	Coin(int type = COIN_TYPE_NORMAL);
	~Coin();
};