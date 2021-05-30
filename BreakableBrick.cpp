#include "BreakableBrick.h"
#include "GameObject.h"

void BreakableBrick::Render()
{
	if (isDestroyed)
		return;
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void BreakableBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CGameObject::Update(dt);
}

void BreakableBrick::Break() {

}