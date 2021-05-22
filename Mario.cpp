#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Portal.h"
#include "StopPoint.h"

#include "GreenBush.h"
#include "QuestionBrick.h"
#include "Block.h"

CMario::CMario(float x, float y) : CGameObject()
{
	//level = MARIO_LEVEL_BIG;
	level = MARIO_LEVEL_SMALL;
	untouchable = 0;
	ax = MARIO_ACCELERATION;
	ay = MARIO_ACCELERATION_JUMP;
	SetState(MARIO_STATE_IDLE);
	SetDirection(MARIO_DIRECT_RIGHT);
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//DebugOut(L"vy::%f\n", vy);
	// Calculate dx, dy 
	CGameObject::Update(dt);
	// Simple fall down
	vy += ay * dt;
	vx += ax * dt;

	//DebugOut(L"vy::%f\n", vy);

	limitMarioSpeed(vx, nx);
	handleMarioJump();

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	if (vy < 0)
		isOnGround = false;

	coEvents.clear();


	//! turn off collision when die 
	//if (state != MARIO_STATE_DIE)
	CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	/*if (GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}*/
	HandleTransform(level);
	HandleChangeDirection();

	// No collision occured, proceed normall
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 
		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;
		//if (nx != 0) vx = 0;
		//if (ny != 0) vy = 0;
		if (vx == 0 && start_running != 0) {
			isChangeDirection = false;
			start_running = 0;
		}

		//
		// Collision logic with other objects
		//
		float oLeft, oTop, oRight, oBottom;
		float mLeft, mTop, mRight, mBottom;
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			GetBoundingBox(mLeft, mTop, mRight, mBottom);
			e->obj->GetBoundingBox(oLeft, oTop, oRight, oBottom);
			if (dynamic_cast<CBrick*>(e->obj) || dynamic_cast<QuestionBrick*>(e->obj)) {
				if (ny != 0) {
					isOnGround = true; vy = 0;
				}
				//else isOnGround = false;
				if (dynamic_cast<QuestionBrick*>(e->obj)) {
					if (e->ny > 0) {
						e->obj->SetState(QUESTION_BRICK_HIT);
						vy = 0;
						ay = MARIO_GRAVITY;
					}
				}
			}
			if (dynamic_cast<Block*>(e->obj))
			{
				if (dynamic_cast<Block*>(e->obj)) {
					if (e->nx != 0 && ceil(mBottom) != oTop)
						x += dx;
					if (e->ny < 0) {
						vy = 0;
						isOnGround = true;
					}
					if (e->ny > 0) {
						y += dy;
					}
				}
			}

			if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (goomba->GetState() != GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
				}
			} // if Goomba
			else if (dynamic_cast<CPortal*>(e->obj))
			{
				CPortal* p = dynamic_cast<CPortal*>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CMario::limitMarioSpeed(float& vx, int nx) {
	if (abs(vx) > MARIO_SPEED_MAX) {
		if (nx > 0) {
			vx = MARIO_SPEED_MAX;
		}
		else vx = -MARIO_SPEED_MAX;
	}
}

void CMario::Render()
{
	//RenderMarioAniSmall();
	int ani = -1;
	if (level == MARIO_LEVEL_SMALL) {
		RenderMarioAniSmall(ani);
	}
	if (level == MARIO_LEVEL_BIG) {
		RenderMarioAniBig(ani);
	}
	if (state == MARIO_STATE_TRANSFORM) {
		if (nx > 0) {
			ani = MARIO_ANI_TRANSFORM_SMALL_RIGHT;
		}
		else ani = MARIO_ANI_TRANSFORM_SMALL_LEFT;
	}
	int alpha = 255;
	if (untouchable) alpha = 128;
	if (state == MARIO_STATE_TRANSFORM) {
		animation_set->at(ani)->Render(x, y - 9, alpha);
	}
	else animation_set->at(ani)->Render(x, y, alpha);

	RenderBoundingBox();
}

void CMario::RenderMarioAniSmall(int& ani) {
	if (state == MARIO_STATE_IDLE) {
		if (nx > 0) {
			ani = MARIO_SMALL_RIGHT_IDLE;
		}
		if (nx < 0) {
			ani = MARIO_SMALL_LEFT_IDLE;
		}
	}
	if (state == MARIO_STATE_WALKING_RIGHT) {
		if (isChangeDirection && abs(vx) > MARIO_WALKING_SPEED_START) {
			ani = MARIO_BRAKE_SMALL_LEFT;
		}
		else if (!isChangeDirection && isOnGround) {
			ani = MARIO_SMALL_WALK_RIGHT;
		}
		else if (!isOnGround) {
			ani = MARIO_JUMP_SMALL_RIGHT;
		}
	}
	if (state == MARIO_STATE_WALKING_LEFT) {
		if (isChangeDirection && abs(vx) > MARIO_WALKING_SPEED_START) {
			ani = MARIO_BRAKE_SMALL_RIGHT;
		}
		else if (!isChangeDirection && isOnGround) {
			ani = MARIO_SMALL_WALK_LEFT;
		}
		else if (!isOnGround) {
			ani = MARIO_JUMP_SMALL_LEFT;
		}

	}
	if (state == MARIO_STATE_JUMP) {
		if (nx > 0) {
			ani = MARIO_JUMP_SMALL_RIGHT;
		}
		if (nx < 0) {
			ani = MARIO_JUMP_SMALL_LEFT;
		}
	}
	if (state == MARIO_STATE_JUMP_X) {
		if (nx > 0) {
			ani = MARIO_JUMP_SMALL_RIGHT;
		}
		if (nx < 0) {
			ani = MARIO_JUMP_SMALL_LEFT;
		}
	}
}

void CMario::RenderMarioAniBig(int& ani) {
	if (state == MARIO_STATE_IDLE) {
		if (nx > 0) {
			ani = MARIO_ANI_BIG_IDLE_RIGHT;
		}
		if (nx < 0) {
			ani = MARIO_ANI_BIG_IDLE_LEFT;
		}
	}
	if (state == MARIO_STATE_WALKING_RIGHT) {
		if (isChangeDirection && abs(vx) > MARIO_WALKING_SPEED_START) {
			ani = MARIO_ANI_BIG_BRAKING_LEFT;
		}
		else if (!isChangeDirection && isOnGround) {
			ani = MARIO_ANI_BIG_WALKING_RIGHT;
		}
		else if (!isOnGround) {
			ani = MARIO_ANI_BIG_JUMPINGUP_RIGHT;
		}
	}
	if (state == MARIO_STATE_WALKING_LEFT) {
		if (isChangeDirection && abs(vx) > MARIO_WALKING_SPEED_START) {
			ani = MARIO_ANI_BIG_BRAKING_RIGHT;
		}
		else if (!isChangeDirection && isOnGround) {
			ani = MARIO_ANI_BIG_WALKING_LEFT;
		}
		else if (!isOnGround) {
			ani = MARIO_ANI_BIG_JUMPINGUP_LEFT;
		}

	}
	if (state == MARIO_STATE_JUMP) {
		if (nx > 0) {
			ani = MARIO_ANI_BIG_JUMPINGUP_RIGHT;
		}
		if (nx < 0) {
			ani = MARIO_ANI_BIG_JUMPINGUP_LEFT;
		}
	}
	if (state == MARIO_STATE_JUMP_X) {
		if (nx > 0) {
			ani = MARIO_ANI_BIG_JUMPINGUP_RIGHT;
		}
		if (nx < 0) {
			ani = MARIO_ANI_BIG_JUMPINGUP_LEFT;
		}
	}
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case MARIO_STATE_IDLE:
		//DebugOut(L"vy::%f\n", vy);
		if (abs(vx) < MARIO_WALKING_SPEED_MIN) {
			ax = 0;
			vx = 0;
		}
		else {
			vx /= 1.15f;
			ax = 0;
		}
		ay = MARIO_GRAVITY;
		//else slowDownVx();
		break;
	case MARIO_STATE_WALKING_RIGHT:
		//DebugOut(L"ax::%f\n", ax);
		if (ax < 0 && isOnGround) {
			isChangeDirection = true;
			runningStack = 0;
		}
		else {
			runningStack++;
		}
		ax = MARIO_ACCELERATION;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (ax > 0 && isOnGround) {
			isChangeDirection = true;
			runningStack = 0;
		}
		else {
			runningStack++;
		}
		ax = -MARIO_ACCELERATION;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		//DebugOut(L"vy::%f\n", vy);
		if (isOnGround) {
			if (vy > -MARIO_JUMP_SPEED_MIN)
				vy = -MARIO_JUMP_SPEED_MIN;
			ay = -MARIO_ACCELERATION_JUMP;
			isJumping = true;
		}
		isOnGround = false;
		break;
	case MARIO_STATE_JUMP_X:
		if (isOnGround) {
			if (vy > -MARIO_JUMP_SPEED_MIN)
				vy = -MARIO_JUMP_SPEED_MIN;
			ay = -MARIO_ACCELERATION_JUMP;
			isJumpingWithXButton = true;
		}
		isOnGround = false;
		break;
	case MARIO_STATE_TRANSFORM:
		vx = 0;
		vy = 0;
		//StartTransform();
		break;
	}

}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;

	if (level == MARIO_LEVEL_BIG)
	{
		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;

		/*	right = x + MARIO_SMALL_BBOX_WIDTH;
			bottom = y + MARIO_SMALL_BBOX_HEIGHT;*/
	}
	else
	{
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
}

/*
	Reset Mario status to the beginning state of a scene
*/
void CMario::Reset()
{
	level = MARIO_LEVEL_SMALL;
	untouchable = 0;
	ax = MARIO_ACCELERATION;
	ay = MARIO_ACCELERATION_JUMP;
	SetState(MARIO_STATE_IDLE);
	SetDirection(MARIO_DIRECT_RIGHT);
	SetPosition(start_x, start_y);
}

void CMario::SetAccelerate(float accelerate = MARIO_ACCELERATION) {
	ax += accelerate;
}

void CMario::HandleChangeDirection() {
	if (isChangeDirection && runningStack > 15) {
		runningStack = 0;
		isChangeDirection = false;
		vx = 0;
	}
}

void CMario::handleMarioJump() {
	if (isJumpingWithXButton) {
		if (vy < -MARIO_JUMP_SPEED_MIN / 2) {
			ay = MARIO_ACCELERATION_JUMP;
			vy = -MARIO_JUMP_SPEED_MIN;
			isJumpingWithXButton = false;
		}
	}
	if (isJumping) {
		//DebugOut(L"ax::%f\n", 0.0f);
		//DebugOut(L"vy::%f\n", vy);
		// Dung yen nhay
		if (vx == 0)
		{
			if (vy < -MARIO_JUMP_MAX) {
				pullDown();
			}
		}
		// Di chuyen nhay phai
		if (vx > 0) {
			// vx lon nhat
			DebugOut(L"vx::%f\n", vx);
			if (vx >= MARIO_SPEED_MAX) {
				// super jump
				if (vy < -MARIO_SUPER_JUMP_MAX) {
					pullDown();
				}
			}
			else if (vx < MARIO_SPEED_MAX && vx > 0) {
				if (vy < -MARIO_JUMP_MAX) {
					pullDown();
				}
			}
		}
		//Di chuyen nhay trai
		if (vx < 0) {
			// vx lon nhat
			if (abs(vx) >= MARIO_SPEED_MAX) {
				// super jump
				if (vy < -MARIO_SUPER_JUMP_MAX) {
					pullDown();
				}
			}
			else if (abs(vx) < MARIO_SPEED_MAX && vx < 0) {
				if (vy < -MARIO_JUMP_MAX) {
					pullDown();
				}
			}
		}

	}
}

void CMario::HandleTransform(int level) {
	if (isTransforming) {
		SetState(MARIO_STATE_TRANSFORM);
		if (GetTickCount() - start_transform > MARIO_TRANSFORM_TIME) {
			StopTransform();
			SetState(MARIO_STATE_JUMP_X);
		}
	}
}