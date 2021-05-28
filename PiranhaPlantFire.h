#pragma once
#include "PiranhaPlant.h"
class PiranhaPlantFire :
	public PiranhaPlant
{
public:
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void SetState(int state);
};