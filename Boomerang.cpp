#include "boomerang.h"

Boomerang::Boomerang(float x, float y) {
	this->start_x = x;
	this->start_y = y;
}

void Boomerang::Render() {
	if (!isAppear) return;
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void Boomerang::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CGameObject::Update(dt);
}

void Boomerang::SetState(int state) {
	CGameObject::SetState(state);

	switch (state)
	{
	default:
		break;
	}
}