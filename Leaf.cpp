#include "Leaf.h"
#include "Mario.h"
#include "PlayScence.h"

void Leaf::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + LEAF_BBOX_WIDTH;
	b = y + LEAF_BBOX_HEIGHT;
}

void Leaf::Render()
{
	if (!isAppear || isDestroyed)
		return;
	int ani = LEAF_ANI_LEFT;
	if (state == LEAF_STATE_FALLING)
	{
		if (vx >= 0)
			ani = LEAF_ANI_RIGHT;
		else
			ani = LEAF_ANI_LEFT;
	}
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void Leaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	CGameObject::Update(dt);

	x += dx;
	y += dy;

	if (!IsInViewPort()) {
		isDestroyed = true;
	}

	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = currentScene->GetPlayer();
	float mLeft, mTop, mRight, mBottom;
	/*if (mario != NULL) {
		mario->GetBoundingBox(mLeft, mTop, mRight, mBottom);
		if (isColliding(mLeft, mTop, mRight, mBottom) && !isDestroyed) {
			mario->StartTransform(MARIO_LEVEL_TAIL);
			isAppear = true;
			isDestroyed = true;
		}
	}*/
	if (state == LEAF_STATE_FALLING) {
		if (GetTickCount64() - start_timing >= LEAF_FALLING_TIME) {
			vx = -vx;
			StartTiming();
		}
	}
	if (state == LEAF_STATE_UP) {
		if (start_y - y >= LEAF_UP_HEIGHT) {
			SetState(LEAF_STATE_FALLING);
		}
	}
}

void Leaf::SetState(int state) {
	CGameObject::SetState(state);
	switch (state)
	{
	case LEAF_STATE_IDLE:
		vx = vy = 0;
		break;
	case LEAF_STATE_UP:
		vy = -LEAF_SPEED_UP;
		vx = 0;
		start_y = y;
		break;
	case LEAF_STATE_FALLING:
		vy = LEAF_GRAVITY;
		vx = LEAF_SPEED;
		StartTiming();
		break;
	}
}