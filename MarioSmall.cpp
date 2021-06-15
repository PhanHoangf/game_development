#include "MarioSmall.h"

CMarioSmall::CMarioSmall(float x, float y) {
	this->start_x = x;
	this->start_y = y;
	this->nx = 1;
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_LEVEL_SMALL);
}

void CMarioSmall::Render() {
	RenderMarioSmall();
}

void CMarioSmall::RenderMarioSmall() {
	int ani = -1;

	if (state == MARIO_STATE_IDLE) {
		if (nx > 0) ani = MARIO_ANI_SMALL_IDLE_RIGHT;
		if (nx < 0) ani = MARIO_ANI_SMALL_IDLE_LEFT;
	}
	if (state == MARIO_STATE_WALKING_RIGHT) {
		ani = MARIO_ANI_SMALL_WALKING_RIGHT;
	}
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();

}

void CMarioSmall::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CGameObject::Update(dt);

	vx += ax * dt;
	vy += ay * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (state != MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);
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
				if (e->ny > 0) {
					vy = 0;
				}
			}
		}
	}
}

void CMarioSmall::SetState(int state) {
	CPlayer::SetState(state);
}

void CMarioSmall::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x;
	top = y;
	right = x + MARIO_SMALL_BBOX_WIDTH;
	bottom = y + MARIO_SMALL_BBOX_HEIGHT;
}