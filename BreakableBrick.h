#pragma once
#include "Brick.h"

#define BREAKABLEBRICK_ANI_SET_ID 12
#define PIECE_ANI_SET_ID	126

class BreakableBrick :
	public CBrick
{
public:
	BreakableBrick(float x, float y, int type, int item, int totalItems) :CBrick(x, y, type, item, totalItems) {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void Break();
	void ChangeToCoin();
};

