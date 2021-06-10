#include "CardItem.h"

CardItem::CardItem() {
	vx = 0;
	vy = 0;
	state = CARD_STATE_RANDOM;
}

void CardItem::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + CARD_BBOX_WIDTH;
	b = t + CARD_BBOX_WIDTH;
}

void CardItem::Render() {
	int ani = CARD_ANI_RANDOM;
	animation_set->at(ani)->Render(x, y);
}

void CardItem::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CGameObject::Update(dt);
	int result = 0 + (rand() % 3);
}