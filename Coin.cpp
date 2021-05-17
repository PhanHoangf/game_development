#include "Coin.h"

Coin::Coin() : CGameObject() {
	//CGameObject::SetTag(tag);
	/*if (tag == COIN_TYPE_INBRICK)
		isAppear = false;
	else
		isAppear = true;
	if (tag == COIN_TYPE_TRANSFORM)
		StartExist();*/
	state = COIN_STATE_IDLE;
	type = IGNORE;
}

void Coin::Render() {
	/*if (!isAppear || isDestroyed)
		return;*/
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void Coin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

}

void Coin::SetState(int state) {

}

void Coin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + COIN_BBOX_WIDTH;
	b = y + COIN_BBOX_HEIGHT;
}
Coin::~Coin() {

}