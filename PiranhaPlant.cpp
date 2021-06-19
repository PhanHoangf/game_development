#include "PiranhaPlant.h"
#include "Mario.h"
#include "Game.h"
#include "PlayScence.h"

void PiranhaPlant::GetBoundingBox(float& left, float& top,
	float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + PIRANHAPLANT_BBOX_WIDTH;
	bottom = y + PIRANHAPLANT_BBOX_HEIGHT;
}

PiranhaPlant::PiranhaPlant()
{
	SetState(PIRANHAPLANT_STATE_INACTIVE);
	//SetState(PIRANHAPLANT_STATE_DARTING);
}

void PiranhaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	if (GetTickCount64() - dying_start >= PIRANHAPLANT_DIYING_TIME && dying_start != 0)
		isDestroyed = true;
	if (state == PIRANHAPLANT_STATE_DEATH)
		return;
	CGameObject::Update(dt, coObjects);

	y += dy;

	if (y <= limitY && vy < 0)
	{
		y = limitY;
		SetState(PIRANHAPLANT_STATE_BITING);
	}
	if (y >= limitY + PIRANHAPLANT_BBOX_HEIGHT && vy > 0)
	{
		y = limitY + PIRANHAPLANT_BBOX_HEIGHT;
		SetState(PIRANHAPLANT_STATE_INACTIVE);
	}
	if (GetTickCount64() - biting_start >= PIRANHAPLANT_BITING_TIME && biting_start != 0)
	{
		if (y == limitY)
			vy = PIRANHAPLANT_DARTING_SPEED;
			//vy = 0;
		biting_start = 0;
	}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario != NULL) {
		float mLeft, mTop, mRight, mBottom;
		float oLeft, oTop, oRight, oBottom;

		int mWidth = mario->GetWidth();

		if ((floor(mario->x) + (float)mWidth + PIRANHAPLANT_ACTIVE_RANGE <= x
			|| ceil(mario->x) >= x + PIRANHAPLANT_BBOX_WIDTH + PIRANHAPLANT_ACTIVE_RANGE)
			&& state == PIRANHAPLANT_STATE_INACTIVE && biting_start == 0)
			SetState(PIRANHAPLANT_STATE_DARTING);
		
		//! Die
		mario->GetTail()->GetBoundingBox(mLeft, mTop, mRight, mBottom);
		
		if (isColliding(floor(mLeft), mTop, ceil(mRight), mBottom) && mario->isTuring) {
			mario->AddScore(x, y, 100);
			SetState(PIRANHAPLANT_STATE_DEATH);
		}

	}
}

void PiranhaPlant::Render()
{
	int ani = PIRANHAPLANT_ANI_DEATH;
	if (state != PIRANHAPLANT_STATE_DEATH && dying_start == 0)
		ani = PIRANHAPLANT_ANI_DARTING;
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void PiranhaPlant::SetState(int _state)
{
	CGameObject::SetState(_state);
	switch (_state)
	{
	case PIRANHAPLANT_STATE_DARTING:
		vy = -PIRANHAPLANT_DARTING_SPEED;
		//SetType(MOVING);
		break;
	case PIRANHAPLANT_STATE_BITING:
		vy = 0;
		StartBitting();
		break;
	case PIRANHAPLANT_STATE_DEATH:
		vy = 0;
		//SetType(IGNORE);
		StartDying();
		break;
	case PIRANHAPLANT_STATE_INACTIVE:
		vy = 0;
		//SetType(IGNORE);
		StartBitting();
		break;
	}
}