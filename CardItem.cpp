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
	if (!isAppear) return;
	int ani = CARD_ANI_RANDOM;
	if (state == 1) {
		ani = CARD_ANI_MUSHROOM;
	}
	if (state == 2) {
		ani = CARD_ANI_FIREFLOWER;
	}
	if (state == 3) {
		ani = CARD_ANI_STAR;
	}
	animation_set->at(ani)->Render(x, y);
}

void CardItem::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CGameObject::Update(dt);
	if (start == 0) {
		start = GetTickCount64();
	}
	if (GetTickCount64() - start > CARD_RANDOM_TIME) {
		state++;
		if (state > 3) {
			state = 1;
		}
		start = 0;
	}
}