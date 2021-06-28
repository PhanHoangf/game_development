#include "Coin.h"
#include "Mario.h"
#include "PlayScence.h"
#include "Game.h"
#include "Point.h"
Coin::Coin(int tag) : CGameObject() {
	CGameObject::SetTag(tag);
	if (tag == COIN_TYPE_INBRICK)
		isAppear = false;
	else
		isAppear = true;
	if (tag == COIN_TYPE_TRANSFORM)
		StartExist();
	state = COIN_STATE_IDLE;
	type = IGNORE;
}

void Coin::Render() {
	if (!isAppear || isDestroyed) {
		return;
	}
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void Coin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	if (isDestroyed)
		return;

	y += dy;
	CGameObject::Update(dt);



	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = currentScene->GetPlayer();

	float mTop, mLeft, mRight, mBottom;
	float oTop, oLeft, oRight, oBottom;
	if (state == COIN_STATE_IDLE) {

	}
	if (state == COIN_STATE_UP)
	{
		if (GetTickCount64() - timing_start >= COIN_FALLING_TIME)
		{
			SetState(COIN_STATE_DOWN);
			StartTiming();
		}
	}
	if (state == COIN_STATE_DOWN)
	{
		if (GetTickCount64() - timing_start >= COIN_FALLING_TIME)
		{
			isAppear = false;
			SetState(COIN_STATE_IDLE);
			isDestroyed = true;
			mario->AddScore(this->x, this->y, 100, false);
			mario->AddCoin();
			/*if (tag == COIN_TYPE_INBRICK)
				currentScene->deleteLastObject();*/
		}
	}




}

void Coin::SetState(int state) {
	CGameObject::SetState(state);
	switch (state)
	{
	case COIN_STATE_IDLE:
		vx = vy = 0;
		break;
	case COIN_STATE_UP:
		vy = -0.2f;
		StartTiming();
		break;
	case COIN_STATE_DOWN:
		vy = 0.2f;
		break;
	}
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