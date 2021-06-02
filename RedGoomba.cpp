#include "RedGoomba.h"
#include "Brick.h"
#include "QuestionBrick.h"
#include "Utils.h"

RedGoomba::RedGoomba() {
	SetIsHaveWings(true);
	SetState(GOOMBA_STATE_RED_WINGSWALKING);
}

void RedGoomba::Render() {
	int ani = -1;
	if (isHaveWings) {
		if (state == GOOMBA_STATE_RED_WINGSWALKING) {
			ani = GOOMBA_RED_ANI_WINGSWALKING;
		}
		if (state == GOOMBA_STATE_RED_JUMPING || state == GOOMBA_STATE_RED_HIGHJUMPING) {
			ani = GOOMBA_RED_ANI_JUMPING;
		}
		//! TEST

	}
	if (state == GOOMBA_STATE_WALKING) {
		ani = GOOMBA_RED_ANI_WALKING;
	}
	if (state == GOOMBA_STATE_DIE) {
		ani = GOOMBA_RED_ANI_DIE;
	}
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}
void RedGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	CGameObject::Update(dt);

	if (GetTickCount64() - GetDyingStart() >= GOOMBA_TIME_DIYING && GetIsDying())
	{
		SetIsDying(false);
		this->isDestroyed = true;
		/*if (dynamic_cast<CIntroScene*> (CGame::GetInstance()->GetCurrentScene()))
			mario->SetState(MARIO_STATE_WALKING_RIGHT);*/
		return;
	}

	//! Update goomba y when die

	vy += ay * dt;


	if (state != GOOMBA_STATE_DIE && isHaveWings) {
		if (GetTickCount64() - GetWalkingStart() >= GOOMBA_RED_TIME_WALKING && GetIsWalking())
		{
			SetIsWalking(false);
			y -= GOOMBA_RED_BBOX_WINGS_HEIGHT - GOOMBA_RED_BBOX_HEIGHT;
			SetState(GOOMBA_STATE_RED_JUMPING);
			jumping_stacks = 0;
		}

		if (vy < -GOOMBA_JUMP_SPEED && state == GOOMBA_STATE_RED_JUMPING) {
			vy = -GOOMBA_JUMP_SPEED;
			ay = GOOMBA_GRAVITY;
		}

		if (vy < -GOOMBA_HIGHJUMP_SPEED && state == GOOMBA_STATE_RED_HIGHJUMPING) {
			vy = -GOOMBA_HIGHJUMP_SPEED;
			ay = GOOMBA_GRAVITY;
		}
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 

	coEvents.clear();

	//if (state != GOOMBA_STATE_DIE) {
	CalcPotentialCollisions(coObjects, coEvents);
	//}

	float mLeft, mTop, mRight, mBottom;
	float oLeft, oTop, oRight, oBottom;

	if (coEvents.size() == 0) {
		x += dx;
		y += dy;
	}
	else {
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		//if (ny != 0) vy = 0;

		for (UINT i = 0; i < coEventsResult.size(); i++) {
			if (state == GOOMBA_STATE_DIE) return;
			LPCOLLISIONEVENT e = coEventsResult[i];
			GetBoundingBox(mLeft, mTop, mRight, mBottom);
			if (e->obj != NULL)
				if (e->obj->isDestroyed == true)
					continue;
			if (dynamic_cast<CBrick*>(e->obj)) {
				CBrick* object = dynamic_cast<CBrick*>(e->obj);
				if (e->obj != NULL)
					object->GetBoundingBox(oLeft, oTop, oRight, oBottom);
				if (e->ny < 0) {
					DebugOut(L"GOOMBA BRICK\n");
					vy = 0;
					isOnGround = true;
					if (isHaveWings && state != GOOMBA_STATE_WALKING) {
						if (!GetIsWalking()) {
							if (jumping_stacks == 3) {
								SetState(GOOMBA_STATE_RED_HIGHJUMPING);
								jumping_stacks = -1;
							}
							else {
								if (jumping_stacks == -1)
									SetState(GOOMBA_STATE_RED_WINGSWALKING);
								else
									SetState(GOOMBA_STATE_RED_JUMPING);
								jumping_stacks++;
							}
						}
						else {
							ay = GOOMBA_GRAVITY;
						}
					}
				}
				if (e->nx != 0)
				{
					if (ceil(mBottom) != oTop)
					{
						vx = -vx;
						this->nx = -this->nx;
					}
				}
			}
			if (dynamic_cast<QuestionBrick*>(e->obj)) {
				if (e->nx != 0)
				{
					vx = -vx;
					this->nx = -this->nx;
				}
				if (e->ny > 0) {
					vy = 0;
				}
			}
			if (dynamic_cast<CGoomba*>(e->obj)) {
				if (e->nx != 0) {
					x += dx;
				}
			}
		}
	}
}

void RedGoomba::SetState(int state) {
	CGameObject::SetState(state);

	switch (state)
	{
	case GOOMBA_STATE_WALKING:
		vx = -GOOMBA_WALKING_SPEED;
		ay = GOOMBA_GRAVITY;
		isJumping = false;
		break;
	case GOOMBA_STATE_RED_WINGSWALKING:
		vx = -GOOMBA_WALKING_SPEED;
		ay = GOOMBA_GRAVITY;
		StartWalking();
		break;
	case GOOMBA_STATE_RED_JUMPING:
		ay = -GOOMBA_JUMP_SPEED;
		isJumping = true;
		isOnGround = false;
		SetIsWalking(false);
		break;
	case GOOMBA_STATE_RED_HIGHJUMPING:
		ay = -GOOMBA_HIGHJUMP_SPEED;
		isJumping = true;
		isOnGround = false;
		break;
	case GOOMBA_STATE_DIE:
		y += GOOMBA_NORMAL_BBOX_WIDTH - GOOMBA_BBOX_HEIGHT_DIE - 1;
		vx = vy = 0;
		StartDying();
		break;
	case GOOMBA_STATE_DIE_BY_TAIL:
		break;
	default:
		break;
	}
}

void RedGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	//right = x + GOOMBA_RED_BBOX_WIDTH;
	right = x + GOOMBA_NORMAL_BBOX_WIDTH;
	if (state == GOOMBA_STATE_DIE)
		bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
	else {
		bottom = y + 16;
		if (state == GOOMBA_STATE_RED_WINGSWALKING)
			bottom = y + GOOMBA_RED_BBOX_HEIGHT;
		if (state == GOOMBA_STATE_RED_JUMPING) {
			bottom = y + GOOMBA_RED_BBOX_WINGS_HEIGHT;
		}
	}
}

