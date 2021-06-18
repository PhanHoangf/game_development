#include "Tail.h"
#include "Mario.h"
#include "PlayScence.h"

void Tail::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + TAIL_BBOX_WIDTH;
	b = t + TAIL_BBOX_HEIGHT;
}

void Tail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (x < mario->x)
		nx = -1;
	else
		nx = 1;
	if (mario->nx > 0)
	{
		if (mario->turningStack == 1 || mario->turningStack == 5)
			x = mario->x - TAIL_BBOX_WIDTH;
		else if (mario->turningStack == 3)
			x = mario->x + MARIO_BIG_BBOX_WIDTH;
		else x = mario->x - 8;
	}
	else
	{
		if (mario->turningStack == 1 || mario->turningStack == 5)
			x = mario->x + MARIO_BIG_BBOX_WIDTH;
		else if (mario->turningStack == 3)
			x = mario->x - TAIL_BBOX_WIDTH;
		else x = mario->x + MARIO_BIG_BBOX_WIDTH;
	}
	//x = mario->x - TAIL_BBOX_WIDTH;
	float mLeft, mTop, mRight, mBottom;
	y = mario->y + TAIL_BBOX_WIDTH * 2 + 1;
	//y = mario->y;
}

void Tail::Render() {
	/*if (hit_start) {
		if (nx < 0)
			CSprites::GetInstance()->Get(TAIL_HIT_SPRITE_ID)->Draw(x - 2, y);
		if (nx > 0)
			CSprites::GetInstance()->Get(TAIL_HIT_SPRITE_ID)->Draw(x + TAIL_BBOX_WIDTH - 2, y);
	}*/
	RenderBoundingBox();
}

void Tail::SetState(int state) {
	CGameObject::SetState(state);
}
