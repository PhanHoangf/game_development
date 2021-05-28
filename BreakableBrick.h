#pragma once
#include "Brick.h"

#define BREAKABLEBRICK_ANI_SET_ID 12

class BreakableBrick :
    public CBrick
{
public:
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void Break();
};

