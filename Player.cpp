#include "Player.h"

CPlayer::CPlayer(float x, float y) {
	this->x = x;
	this->y = y;
	this->start_x = x;
	this->start_y = y;
	ax = MARIO_ACCELERATION;
	ay = MARIO_GRAVITY;
	nx = 1;
	level = MARIO_LEVEL_SMALL;
	SetState(MARIO_STATE_IDLE);
}

void CPlayer::SetState(int state) {
	CGameObject::SetState(state);
	switch (state)
	{
	case MARIO_STATE_IDLE:
		if (vx > 0) {
			ax = -MARIO_ACCELERATION;
		}
		if (vx < 0) {
			ax = MARIO_ACCELERATION;
		}
		ay = MARIO_GRAVITY;
		if (abs(vx) <= MARIO_WALKING_SPEED_MIN)
		{
			vx = 0;
			ax = 0;
		}
		break;
	case MARIO_STATE_WALKING_RIGHT:
		if (ax < 0 && vy > 0)
		{
			isChangeDirection = true;
		}
		ax = MARIO_ACCELERATION;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (ax > 0 && vy > 0)
		{
			isChangeDirection = true;
		}
		ax = -MARIO_ACCELERATION;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		if (vy > -MARIO_JUMP_SPEED_MIN)
			vy = -MARIO_JUMP_SPEED_MIN;
		ay = -MARIO_ACCELERATION_JUMP;
		break;
	}
}

void CPlayer::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {


	DebugOut(L"m->vx::%f\n", vx);

	CGameObject::Update(dt);

	vx += ax * dt;
	vy += ay * dt;



	if (vy < 0) {
		isOnGround = false;
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (state != MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	LimitWalkingSpeed();
	LimitJump();


	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
		/*if (y - lastStandingY > 1.0f)
			isOnGround = false;*/
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
		float oLeft, oTop, oRight, oBottom;
		float mLeft, mTop, mRight, mBottom;

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		for (UINT i = 0; i < coEventsResult.size(); i++) {
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CBrick*>(e->obj)) {
				if (e->ny < 0) {
					vy = 0;
					isOnGround = true;
					isReadyToJump = true;
				}
			}
		}
	}
}

void CPlayer::LimitWalkingSpeed() {
	if (abs(vx) >= MARIO_WALKING_SPEED_MAX) {
		if (!isRunning) {
			vx = nx * MARIO_WALKING_SPEED_MAX;
		}
	}
}

void CPlayer::LimitJump() {

	if (vy > MARIO_JUMP_SPEED_MAX) {
		vy = MARIO_JUMP_SPEED_MAX;
		ay = MARIO_GRAVITY;
	}

	if (vy <= -MARIO_JUMP_SPEED_MAX) {
		vy = -MARIO_JUMP_SPEED_MAX;
		ay = MARIO_GRAVITY;
		isReadyToJump = false;
	}

}