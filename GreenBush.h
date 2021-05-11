#pragma once
#include "GameObject.h"
#include "Brick.h"

class GreenBush : public CGameObject
{
	float start_x;
	float start_y;
public:
	virtual void Render();

	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) {
		l = x;
		t = y;
		r = x + BRICK_BBOX_WIDTH;
		b = y + BRICK_BBOX_HEIGHT;
	};
};

