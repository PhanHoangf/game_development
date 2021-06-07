#include "WorldPlayer.h"
#include "StopPoint.h"

WorldPlayer::WorldPlayer(float x, float y) :CGameObject() {
	level = 0; //! MARIO_LEVEL_SMALL
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
	SetState(PLAYER_STATE_IDLE);
}

void WorldPlayer::Render() {
	animation_set->at(level)->Render(x, y);
	RenderBoundingBox();
}

void WorldPlayer::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CGameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0) {
		x += dx;
		y += dy;
	}
	else {
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x +=  min_tx * dx + nx * 0.4f;
		y +=  min_ty * dy + ny * 0.4f;
		SetState(PLAYER_STATE_IDLE);
		for (UINT i = 0; i < coEventsResult.size(); i++) {
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (e->obj != NULL) {
				x = e->obj->x;
				y = e->obj->y;
			}
			if (dynamic_cast<StopPoint*>(e->obj)) {
				
			}
		}
	}
}

void WorldPlayer::SetState(int state) {
	switch (state)
	{
	case PLAYER_STATE_IDLE:
		vx = vy = 0;
		break;
	case PLAYER_STATE_RIGHT:
		vx = PLAYER_SPEED;
		vy = 0;
		break;
	case PLAYER_STATE_LEFT:
		vx = -PLAYER_SPEED;
		vy = 0;
		break;
	case PLAYER_STATE_UP:
		vx = 0;
		vy = -PLAYER_SPEED;
		break;
	case PLAYER_STATE_DOWN:
		vx = 0;
		vy = PLAYER_SPEED;
		break;
	}
}

void WorldPlayer::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x + 2;
	top = y + 2;
	right = left + PLAYER_BBOX_WIDTH - 4;
	bottom = top + PLAYER_BBOX_WIDTH - 4;
}