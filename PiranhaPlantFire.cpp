#include "PiranhaPlantFire.h"
#include "Mario.h"
#include "PlayScence.h"
#include "FireBullet.h"

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

	if (GetTickCount64() - dying_start >= PIRANHAPLANT_DIYING_TIME && dying_start != 0)
		isDestroyed = true;

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
		y = limitY + BBHeight + 12;
		SetState(PIRANHAPLANT_STATE_INACTIVE);
		StartDelay();
	}
	/*int res = GetTickCount64() - aim_start >= PIRANHAPLANT_AIM_TIME && aim_start != 0 ? 1 : 0;
	DebugOut(L"[FirePiran_aim_start]::%d\n", aim_start);
	DebugOut(L"[GetTickCount64()]::%d\n", GetTickCount64());
	DebugOut(L"[res]::%d\n", res);*/
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
		/*if ((floor(mario->x) + (float)mWidth + PIRANHAPLANT_ACTIVE_RANGE <= x
			|| ceil(mario->x) >= x + PIRANHAPLANT_BBOX_WIDTH + PIRANHAPLANT_ACTIVE_RANGE)
			&& state == PIRANHAPLANT_STATE_INACTIVE && delay_start == 0)
			SetState(PIRANHAPLANT_STATE_DARTING);*/
		//! 60 is red piranha active zone
		if (x - 60 <= floor(mario->x)
			&& floor(mario->x) + (float)mWidth <= x
			&& state == PIRANHAPLANT_STATE_INACTIVE && delay_start == 0
			&& !isMarioInActiveZone
			|| floor(mario->x) > x && floor(mario->x) < x + PIRANHAPLANT_BBOX_WIDTH + 60
			&& state == PIRANHAPLANT_STATE_INACTIVE && delay_start == 0
			&& !isMarioInActiveZone)
		{
			SetState(PIRANHAPLANT_STATE_DARTING);
			isMarioInActiveZone = true;
		}
		if (floor(mario->x) < x - 60 || floor(mario->x) > x + PIRANHAPLANT_BBOX_WIDTH + 60) {
			isMarioInActiveZone = false;
		}

		if (mario->GetLevel() == MARIO_LEVEL_TAIL) {
			mario->GetTail()->GetBoundingBox(mLeft, mTop, mRight, mBottom);

			if (isColliding(floor(mLeft), mTop, ceil(mRight), mBottom) && mario->isTuring) {
				mario->AddScore(x, y, 100);
				SetState(PIRANHAPLANT_STATE_DEATH);
				mario->GetTail()->ShowHitEffect();
			}
		}
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
		Shoot();
		break;
	case PIRANHAPLANT_STATE_DEATH:
		vy = 0;
		//SetType(IGNORE);
		StartDying();
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
};

void PiranhaPlantFire::Shoot() {
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	this->bullet = new FireBullet(x, y, Up, Right);

	//! Basic setup for bullet object
	int ani_set_id = BULLET_ANI_SET_ID;
	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
	bullet->SetAnimationSet(ani_set);
	currentScene->AddSpecialObject(bullet);
}