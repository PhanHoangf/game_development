#pragma once
#include "Goomba.h"

class RedGoomba :
	public CGoomba
{
	int runningStack = 0;
	bool isJumping = false;
	bool isOnGround = false;
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	bool isWhackedDying = false;
public:
	bool isHaveWings = true;
	RedGoomba();
	void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void SetIsHaveWings(bool haveWings) { isHaveWings = haveWings; }
};

