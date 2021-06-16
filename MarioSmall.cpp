#include "MarioSmall.h"

CMarioSmall::CMarioSmall(float x, float y) :CPlayer(x, y) {}

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
		if (vx < 0 && nx > 0) {
			ani = MARIO_ANI_SMALL_BRAKING_LEFT;
		}
		else if (vx > 0 && nx > 0) {
			ani = MARIO_ANI_SMALL_WALKING_RIGHT;
		}
	}
	if (state == MARIO_STATE_WALKING_LEFT) {
		if (vx > 0 && nx < 0) {
			ani = MARIO_ANI_SMALL_BRAKING_RIGHT;
		}
		else if (vx < 0 && nx < 0) {
			ani = MARIO_ANI_SMALL_WALKING_LEFT;
		}
	}
	if (state == MARIO_STATE_JUMP) {
		if (nx > 0) {
			ani = MARIO_ANI_SMALL_JUMPINGUP_RIGHT;
		}
		else if (nx < 0) {
			ani = MARIO_ANI_SMALL_JUMPINGUP_LEFT;
		}
	}
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();

}

void CMarioSmall::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CPlayer::Update(dt, coObjects);
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

void CMarioSmall::MarioSmallLimit() {
	LimitWalkingSpeed();
}