#pragma once
#include "GameObject.h"
#include "Brick.h"

#define GROUND_BBOX_WIDTH  300
#define GROUND_BBOX_HEIGHT 16

class IntroGround : public CBrick {
	void GetBoundingBox(float& l, float& t, float& r, float& b) {
		l = x;
		t = y;
		r = x + GROUND_BBOX_WIDTH;
		b = y + GROUND_BBOX_HEIGHT;
	};
};