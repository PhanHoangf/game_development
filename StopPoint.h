#pragma once
#include "GameObject.h"

#define STOP_POINT_WIDTH 16

class StopPoint :
	public CGameObject
{
	virtual void Render() {
		animation_set->at(0)->Render(x, y);

		RenderBoundingBox();
	};

	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) {
		l = x;
		t = y;
		r = x + STOP_POINT_WIDTH;
		b = y + STOP_POINT_WIDTH;
	};
};

