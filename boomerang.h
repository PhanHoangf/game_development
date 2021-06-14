#pragma once

#include "GameObject.h"

#define BOOMERANG_BBOX_WIDTH	16
#define BOOMERANG_BBOX_HEIGHT	16

class Boomerang :public CGameObject {
	bool isAppear = false;
public:
	Boomerang(float x, float y);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	void SetState(int state);

	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) {
		l = x;
		t = y;
		r = x + BOOMERANG_BBOX_WIDTH;
		b = y + BOOMERANG_BBOX_HEIGHT;
	};

	void SetIsAppear(bool ap) { this->isAppear = ap; }
	bool GetIsAppear() { return this->isAppear; }
};