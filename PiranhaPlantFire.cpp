#include "PiranhaPlantFire.h"
#include "Mario.h"
#include "PlayScence.h"

PiranhaPlantFire::PiranhaPlantFire(int tag) {
	this->tag = tag;
	SetState(PIRANHAPLANT_STATE_INACTIVE);
}

void PiranhaPlantFire::Render()
{
	int ani = PIRANHAPLANT_ANI_DEATH;
	if (state != PIRANHAPLANT_STATE_DEATH && dying_start == 0)
	{
		if (Up)
			if (Right)
				ani = PIRANHAPLANT_ANI_RIGHT_UP;
			else
				ani = PIRANHAPLANT_ANI_LEFT_UP;
		else
			if (Right)
				ani = PIRANHAPLANT_ANI_RIGHT_DOWN;
			else
				ani = PIRANHAPLANT_ANI_LEFT_DOWN;
	}
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void PiranhaPlantFire::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	if (state == PIRANHAPLANT_STATE_DEATH)
		return;

	CGameObject::Update(dt);
	y += dy;
	GetDirect();

	if (y <= limitY && vy < 0)
	{
		y = limitY;
		vy = 0;
		StartAim();
	}

	if (y >= limitY + BBHeight && vy > 0)
	{
		y = limitY + BBHeight;
		SetState(PIRANHAPLANT_STATE_INACTIVE);
		StartDelay();
	}

	if (GetTickCount64() - aim_start >= PIRANHAPLANT_AIM_TIME && aim_start != 0)
	{
		aim_start = 0;
		SetState(PIRANHAPLANT_STATE_SHOOTING);
		StartDelay();
	}

	if (GetTickCount64() - delay_start >= PIRANHAPLANT_DELAY_TIME && delay_start != 0)
	{
		delay_start = 0;
		if (y == limitY)
			vy = PIRANHAPLANT_DARTING_SPEED;
	}

	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario != NULL) {
		float mLeft, mTop, mRight, mBottom;
		float oLeft, oTop, oRight, oBottom;

		int mWidth = mario->GetWidth();
		if ((floor(mario->x) + (float)mWidth + PIRANHAPLANT_ACTIVE_RANGE <= x
			|| ceil(mario->x) >= x + PIRANHAPLANT_BBOX_WIDTH + PIRANHAPLANT_ACTIVE_RANGE)
			&& state == PIRANHAPLANT_STATE_INACTIVE && delay_start == 0)
			SetState(PIRANHAPLANT_STATE_DARTING);
	}
}


void PiranhaPlantFire::GetBoundingBox(float& left, float& top,
	float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + PIRANHAPLANT_BBOX_WIDTH;
	bottom = y + BBHeight;
}

void PiranhaPlantFire::SetState(int state) {
	CGameObject::Update(state);
	switch (state)
	{
	case PIRANHAPLANT_STATE_DARTING:
		vy = -PIRANHAPLANT_DARTING_SPEED;
		break;
	case PIRANHAPLANT_STATE_SHOOTING:
		vy = 0;
		//Shoot();
		break;
	case PIRANHAPLANT_STATE_INACTIVE:
		vy = 0;
		break;
	}
}

void PiranhaPlantFire::GetDirect() {
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	int mHeight;
	if (mario->GetLevel() == MARIO_LEVEL_SMALL)
		mHeight = MARIO_SMALL_BBOX_HEIGHT;
	else
		mHeight = MARIO_BIG_BBOX_HEIGHT;

	if (mario->y + mHeight < limitY + BBHeight)
		Up = true;
	else
		Up = false;
	if (mario->x <= x)
		Right = false;
	else
		Right = true;

	int r = Right ? 1 : 0;
	int u = Up ? 1 : 0;
};