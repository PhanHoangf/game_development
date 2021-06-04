#include "Tail.h"

void Tail::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + TAIL_BBOX_WIDTH;
	b = t + TAIL_BBOX_HEIGHT;
}

void Tail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

}

void Tail::Render() {
	if (nx < 0)
		CSprites::GetInstance()->Get(TAIL_HIT_SPRITE_ID)->Draw(x - 2, y);
	if (nx > 0)
		CSprites::GetInstance()->Get(TAIL_HIT_SPRITE_ID)->Draw(x + TAIL_BBOX_WIDTH - 2, y);
}

void Tail::SetState(int state) {
	CGameObject::SetState(state);
}
