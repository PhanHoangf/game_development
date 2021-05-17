#include "QuestionBrick.h"
#include "Utils.h"

QuestionBrick::QuestionBrick(int tag, int type) : CGameObject() {
	state = QUESTION_BRICK_NORMAL;
}

void QuestionBrick::Render() {
	int ani = -1;

	if (state == QUESTION_BRICK_NORMAL) {
		ani = QUESTION_BRICK_ANI_NORMAL;
	}
	if (state == QUESTION_BRICK_HIT) {
		ani = QUESTION_BRICK_ANI_HIT;
	}
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void QuestionBrick::GetBoundingBox(float& l, float& t, float& r, float& b) {
	l = x;
	t = y;
	r = x + 16;
	b = y + 16;
}

void QuestionBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CGameObject::Update(dt);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	if (coEvents.size() == 0)
	{
		y += dy;
	}
	else {

	}
	if (state == QUESTION_BRICK_HIT) {
		if (isBeingPushedUp && start_y - y >= QUESTIONBRICK_PUSH_MAX_HEIGHT) {
			stopPushedUp();
		}
		if (isFallingDown && y >= start_y) {
			y = start_y;
			isFallingDown = false;
			vy = 0;
		}
	}

	//DebugOut(L"[BRICK vy]::%f\n", vy);
}

void QuestionBrick::SetItem(LPGAMEOBJECT item) {
	this->item = item;
}

void QuestionBrick::startPushedUp() {
	isBeingPushedUp = true;
	vy = -QUESTIONBRICK_SPEED;
}

void QuestionBrick::stopPushedUp() {
	isBeingPushedUp = false;
	isFallingDown = true;
	vy = QUESTIONBRICK_SPEED;
}

void QuestionBrick::SetState(int state) {
	CGameObject::SetState(state);
	switch (state)
	{
	case QUESTION_BRICK_NORMAL:
		vy = 0;
		break;
	case QUESTION_BRICK_HIT:
		startPushedUp();
		break;
	}
}