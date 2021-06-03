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
#include "Koopas.h"
#include "Coin.h"
#include "Switch.h"
#include "FireBullet.h"
#include "PiranhaPlantFire.h"
#include "RedGoomba.h"

CMario::CMario(float x, float y) : CGameObject()
{
	level = MARIO_LEVEL_BIG;
	//level = MARIO_LEVEL_SMALL;
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
	if (state != MARIO_STATE_DIE) {
		CalcPotentialCollisions(coObjects, coEvents);
	}

	HandleTransform(level);
	HandleChangeDirection();
	HandleMarioKicking();
	HandleMarioHolding();

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}


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
			if (state == MARIO_STATE_DIE) return;
			LPCOLLISIONEVENT e = coEventsResult[i];
			GetBoundingBox(mLeft, mTop, mRight, mBottom);
			e->obj->GetBoundingBox(oLeft, oTop, oRight, oBottom);

			if (dynamic_cast<CBrick*>(e->obj) || dynamic_cast<QuestionBrick*>(e->obj)) {
				if (e->ny < 0) {
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
				if (e->nx != 0 && ceil(mBottom) != oTop) {
					vx = 0;
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
			if (dynamic_cast<CGoomba*>(e->obj) && !dynamic_cast<RedGoomba*>(e->obj)) // if e->obj is Goomba 
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
				if (e->nx != 0) {
					if (untouchable != 1) {
						if (level == MARIO_LEVEL_SMALL) {
							SetState(MARIO_STATE_DIE);
						}
						//! 1: Set back to level small -> 2: start untouchable
						if (level == MARIO_LEVEL_BIG) {
							SetLevel(MARIO_LEVEL_SMALL);
							StartUntouchable();
						}
					}
					else {
						x += dx;
					}
				}
			} // if Goomba
			if (dynamic_cast<CKoopas*>(e->obj)) {
				CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
				if (e->nx != 0) {
					if (koopas->GetState() == KOOPAS_STATE_WALKING || koopas->GetState() == KOOPAS_STATE_SPINNING) {
						if (untouchable != 1) {
							if (level == MARIO_LEVEL_SMALL) SetState(MARIO_STATE_DIE);
							else if (level == MARIO_LEVEL_BIG) {
								StartUntouchable();
								SetLevel(MARIO_LEVEL_SMALL);
							}
						}
						else x += dx;
					}
					if (koopas->GetState() == KOOPAS_STATE_IN_SHELL) {
						DebugOut(L"collision with KOOPAS\n");
						if (isReadyToHold) {
							isHolding = true;
							koopas->SetIsBeingHeld(true);
						}
						else {
							koopas->SetIsBeingHeld(false);
							koopas->SetState(KOOPAS_STATE_SPINNING);
							SetState(MARIO_STATE_KICK);
						}
					}
				}
				if (e->ny < 0) {
					vy = -MARIO_JUMP_DEFLECT_SPEED;
					if (koopas->GetState() == KOOPAS_STATE_WALKING)
						koopas->SetState(KOOPAS_STATE_IN_SHELL);
					else if (koopas->GetState() == KOOPAS_STATE_IN_SHELL) {
						koopas->SetState(KOOPAS_STATE_SPINNING);
					}
					else if (koopas->GetState() == KOOPAS_STATE_SPINNING) {
						koopas->SetState(KOOPAS_STATE_IN_SHELL);
					}
				}
			}
			if (dynamic_cast<Coin*>(e->obj)) {
				Coin* coin = dynamic_cast<Coin*>(e->obj);
				if (e->ny != 0 || e->nx != 0) {
					coin->SetAppear(false);
					coin->SetIsDestroyed(true);
				}
			}
			if (dynamic_cast<Switch*>(e->obj)) {
				Switch* sw = dynamic_cast<Switch*>(e->obj);
				if (e->ny < 0) {
					if (sw->GetState() != SWITCH_STATE_PRESSED)
						sw->SetState(SWITCH_STATE_PRESSED);
					vy = -MARIO_JUMP_DEFLECT_SPEED;
				}
			}
			if (dynamic_cast<FireBullet*>(e->obj)) {
				if (e->ny != 0 || e->nx != 0) {
					HandleBasicMarioDie();
				}
			}
			if (dynamic_cast<PiranhaPlantFire*>(e->obj) || dynamic_cast<PiranhaPlant*>(e->obj)) {
				if (e->ny != 0 || e->nx != 0) {
					HandleBasicMarioDie();
				}
			}
			if (dynamic_cast<RedGoomba*>(e->obj)) {
				RedGoomba* rGoomba = dynamic_cast<RedGoomba*>(e->obj);
				if (e->ny < 0) {
					if (!rGoomba->isHaveWings) {
						rGoomba->SetState(GOOMBA_STATE_DIE);
					}
					else {
						rGoomba->SetIsHaveWings(false);
						rGoomba->SetState(GOOMBA_STATE_WALKING);
					}
					vy = -MARIO_JUMP_DEFLECT_SPEED;
				}
				if (e->nx != 0) {
					HandleBasicMarioDie();
				}
				if (e->ny > 0) {
					HandleBasicMarioDie();
				}
			}
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
	if (level == MARIO_LEVEL_TAIL) {
		RenderMarioAniTail(ani);
	}
	if (state == MARIO_STATE_TRANSFORM) {
		if (nx > 0) {
			ani = MARIO_ANI_TRANSFORM_SMALL_RIGHT;
		}
		else ani = MARIO_ANI_TRANSFORM_SMALL_LEFT;
		if (level == MARIO_LEVEL_TAIL) ani = MARIO_ANI_TRANSFORM_BANG;
	}
	int alpha = 255;
	if (untouchable) alpha = 128;
	if (state == MARIO_STATE_TRANSFORM) {
		if (level == MARIO_LEVEL_SMALL) animation_set->at(ani)->Render(x, y, alpha);
		else if (level == MARIO_LEVEL_BIG) animation_set->at(ani)->Render(x, y, alpha);
		else if (level == MARIO_LEVEL_TAIL) animation_set->at(ani)->Render(x, y, alpha);
	}
	else if (state == MARIO_STATE_SITDOWN) {
		animation_set->at(ani)->Render(x, y, alpha);
	}
	else animation_set->at(ani)->Render(x, y, alpha);

	RenderBoundingBox();
}

void CMario::RenderMarioAniSmall(int& ani) {
	if (state == MARIO_STATE_IDLE) {
		if (isHolding) {
			if (nx > 0) {
				ani = MARIO_ANI_SMALL_HOLD_IDLE_RIGHT;
			}
			if (nx < 0) {
				ani = MARIO_ANI_SMALL_HOLD_IDLE_LEFT;
			}
		}
		else {
			if (nx > 0) {
				ani = MARIO_SMALL_RIGHT_IDLE;
			}
			if (nx < 0) {
				ani = MARIO_SMALL_LEFT_IDLE;
			}
		}
	}
	if (state == MARIO_STATE_WALKING_RIGHT) {
		if (isChangeDirection && abs(vx) > MARIO_WALKING_SPEED_MIN) {
			ani = MARIO_BRAKE_SMALL_LEFT;
		}
		if (!isChangeDirection && isOnGround || abs(vx) < MARIO_WALKING_SPEED_MIN && isOnGround) {
			ani = MARIO_SMALL_WALK_RIGHT;
		}
		if (!isOnGround) {
			ani = MARIO_JUMP_SMALL_RIGHT;
		}
		if (isKicking) {
			ani = MARIO_ANI_SMALL_KICKING_RIGHT;
		}
		if (isHolding) {
			ani = MARIO_ANI_SMALL_HOLD_WALKING_RIGHT;
		}
	}
	if (state == MARIO_STATE_WALKING_LEFT) {
		if (isChangeDirection && abs(vx) > MARIO_WALKING_SPEED_MIN) {
			ani = MARIO_BRAKE_SMALL_RIGHT;
		}
		if (!isChangeDirection && isOnGround || abs(vx) < MARIO_WALKING_SPEED_MIN && isOnGround) {
			ani = MARIO_SMALL_WALK_LEFT;
		}
		if (!isOnGround) {
			ani = MARIO_JUMP_SMALL_LEFT;
		}
		if (isKicking) {
			ani = MARIO_ANI_SMALL_KICKING_LEFT;
		}
		if (isHolding) {
			ani = MARIO_ANI_SMALL_HOLD_WALKING_LEFT;
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
	if (state == MARIO_STATE_DIE)
		ani = MARIO_ANI_DIE;
	if (state == MARIO_STATE_KICK) {
		if (isKicking)
		{
			if (nx > 0) ani = MARIO_ANI_SMALL_KICKING_RIGHT;
			if (nx < 0) ani = MARIO_ANI_SMALL_KICKING_LEFT;
		}
	}
}

void CMario::RenderMarioAniBig(int& ani) {
	if (state == MARIO_STATE_IDLE) {
		//DebugOut(L"hold::%d\n", hold);
		if (isHolding) {
			if (nx > 0) {
				ani = MARIO_ANI_BIG_HOLD_IDLE_RIGHT;
			}
			if (nx < 0) {
				ani = MARIO_ANI_BIG_HOLD_IDLE_LEFT;
			}
		}
		else {
			if (nx > 0) {
				ani = MARIO_ANI_BIG_IDLE_RIGHT;
			}
			if (nx < 0) {
				ani = MARIO_ANI_BIG_IDLE_LEFT;
			}
		}
	}
	if (state == MARIO_STATE_WALKING_RIGHT) {
		if (isChangeDirection && abs(vx) > MARIO_WALKING_SPEED_MIN) {
			ani = MARIO_ANI_BIG_BRAKING_LEFT;
		}
		if (!isChangeDirection && isOnGround || abs(vx) < MARIO_WALKING_SPEED_MIN && isOnGround) {
			ani = MARIO_ANI_BIG_WALKING_RIGHT;
		}
		if (!isOnGround) {
			ani = MARIO_ANI_BIG_JUMPINGUP_RIGHT;
		}
		if (isKicking) {
			ani = MARIO_ANI_BIG_KICKING_RIGHT;
		}
		if (isHolding) {
			ani = MARIO_ANI_BIG_HOLD_WALKING_RIGHT;
		}
	}
	if (state == MARIO_STATE_WALKING_LEFT) {
		if (isChangeDirection && abs(vx) > MARIO_WALKING_SPEED_MIN) {
			ani = MARIO_ANI_BIG_BRAKING_RIGHT;
		}
		if (!isChangeDirection && isOnGround || abs(vx) < MARIO_WALKING_SPEED_MIN && isOnGround) {
			ani = MARIO_ANI_BIG_WALKING_LEFT;
		}
		if (!isOnGround) {
			ani = MARIO_ANI_BIG_JUMPINGUP_LEFT;
		}
		if (isKicking) {
			ani = MARIO_ANI_BIG_KICKING_LEFT;
		}
		if (isHolding) {
			ani = MARIO_ANI_BIG_HOLD_WALKING_LEFT;
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
	if (state == MARIO_STATE_SITDOWN) {
		RenderMarioSit(ani);
	}
	if (state == MARIO_STATE_KICK) {
		if (isKicking)
		{
			if (nx > 0) ani = MARIO_ANI_BIG_KICKING_RIGHT;
			if (nx < 0) ani = MARIO_ANI_BIG_KICKING_LEFT;
		}
	}
	if (state == MARIO_STATE_HOLD) {
		if (isHolding) {
			if (nx > 0) ani = MARIO_ANI_BIG_HOLD_WALKING_RIGHT;
			if (nx < 0) ani = MARIO_ANI_BIG_HOLD_WALKING_LEFT;
		}
	}
}

void CMario::RenderMarioAniTail(int& ani) {
	if (state == MARIO_STATE_IDLE) {
		//DebugOut(L"hold::%d\n", hold);
		if (isHolding) {
			if (nx > 0) {
				ani = MARIO_ANI_BIG_HOLD_IDLE_RIGHT;
			}
			if (nx < 0) {
				ani = MARIO_ANI_BIG_HOLD_IDLE_LEFT;
			}
		}
		else {
			if (nx > 0) {
				ani = MARIO_ANI_TAIL_IDLE_RIGHT;
			}
			if (nx < 0) {
				ani = MARIO_ANI_TAIL_IDLE_LEFT;
			}
		}
	}
	if (state == MARIO_STATE_WALKING_RIGHT) {
		if (isChangeDirection && abs(vx) > MARIO_WALKING_SPEED_MIN) {
			ani = MARIO_ANI_BIG_BRAKING_LEFT;
		}
		if (!isChangeDirection && isOnGround || abs(vx) < MARIO_WALKING_SPEED_MIN && isOnGround) {
			ani = MARIO_ANI_BIG_WALKING_RIGHT;
		}
		if (!isOnGround) {
			ani = MARIO_ANI_BIG_JUMPINGUP_RIGHT;
		}
		if (isKicking) {
			ani = MARIO_ANI_BIG_KICKING_RIGHT;
		}
		if (isHolding) {
			ani = MARIO_ANI_BIG_HOLD_WALKING_RIGHT;
		}
	}
	if (state == MARIO_STATE_WALKING_LEFT) {
		if (isChangeDirection && abs(vx) > MARIO_WALKING_SPEED_MIN) {
			ani = MARIO_ANI_BIG_BRAKING_RIGHT;
		}
		if (!isChangeDirection && isOnGround || abs(vx) < MARIO_WALKING_SPEED_MIN && isOnGround) {
			ani = MARIO_ANI_BIG_WALKING_LEFT;
		}
		if (!isOnGround) {
			ani = MARIO_ANI_BIG_JUMPINGUP_LEFT;
		}
		if (isKicking) {
			ani = MARIO_ANI_BIG_KICKING_LEFT;
		}
		if (isHolding) {
			ani = MARIO_ANI_BIG_HOLD_WALKING_LEFT;
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
	if (state == MARIO_STATE_SITDOWN) {
		RenderMarioSit(ani);
	}
	if (state == MARIO_STATE_KICK) {
		if (isKicking)
		{
			if (nx > 0) ani = MARIO_ANI_BIG_KICKING_RIGHT;
			if (nx < 0) ani = MARIO_ANI_BIG_KICKING_LEFT;
		}
	}
	if (state == MARIO_STATE_HOLD) {
		if (isHolding) {
			if (nx > 0) ani = MARIO_ANI_BIG_HOLD_WALKING_RIGHT;
			if (nx < 0) ani = MARIO_ANI_BIG_HOLD_WALKING_LEFT;
		}
	}
}

void CMario::SetState(int state)
{
	//DebugOut(L"state::%d\n", state);
	int previousState = GetState();
	CGameObject::SetState(state);
	switch (state)
	{
	case MARIO_STATE_IDLE:
		if (abs(vx) < MARIO_WALKING_SPEED_MIN) {
			ax = 0;
			vx = 0;
		}
		else {
			vx /= 1.15f;
			ax = 0;
		}
		ay = MARIO_GRAVITY;
		isSitting = false;
		if (previousState == MARIO_STATE_SITDOWN)
			y -= MARIO_BIG_BBOX_HEIGHT - MARIO_BBOX_SIT_HEIGHT;
		//SetIsReadyToHold(false);
		break;
	case MARIO_STATE_WALKING_RIGHT:
		if (ax < 0 && isOnGround && abs(vx) > MARIO_WALKING_SPEED_START) {
			isChangeDirection = true;
			runningStack = 0;
		}
		else {
			runningStack++;
		}
		ax = MARIO_ACCELERATION;
		nx = 1;
		isSitting = false;
		if (previousState == MARIO_STATE_SITDOWN)
			y -= MARIO_BIG_BBOX_HEIGHT - MARIO_BBOX_SIT_HEIGHT;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (ax > 0 && isOnGround && abs(vx) > MARIO_WALKING_SPEED_START) {
			isChangeDirection = true;
			runningStack = 0;
		}
		else {
			runningStack++;
		}
		ax = -MARIO_ACCELERATION;
		nx = -1;
		isSitting = false;
		if (previousState == MARIO_STATE_SITDOWN)
			y -= MARIO_BIG_BBOX_HEIGHT - MARIO_BBOX_SIT_HEIGHT;
		break;
	case MARIO_STATE_JUMP:
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
		break;
	case MARIO_STATE_SITDOWN:
		handleSitDown();
		break;
	case MARIO_STATE_DIE:
		vx = 0;
		ax = 0;
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	case MARIO_STATE_KICK:
		StartKicking();
		break;
	case MARIO_STATE_READY_TO_HOLD:
		if (isOnGround) isReadyToHold = true;
		break;
	}
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	if (level == MARIO_LEVEL_BIG || level == MARIO_LEVEL_TAIL)
	{

		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
		if (state == MARIO_STATE_SITDOWN) {
			bottom = top + MARIO_BBOX_SIT_HEIGHT;
		}
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
		HandleChangeYTransform();
		if (GetTickCount() - start_transform > MARIO_TRANSFORM_TIME) {
			StopTransform();
			//SetState(MARIO_STATE_JUMP_X);
		}
	}
}

void CMario::handleSitDown() {
	if (isOnGround) {
		isReadyToSit = true;
		//if (isReadyToSit) {
		if (!isSitting) {
			y += MARIO_BIG_BBOX_HEIGHT - MARIO_BBOX_SIT_HEIGHT;
			isSitting = true;
		}
		//}
		vx /= 1.15f;
		ax = 0;
	}
	else {
		isReadyToSit = false;
		isSitting = false;
		//y = y + MARIO_BIG_BBOX_HEIGHT;
	}
	if (level == MARIO_LEVEL_SMALL) {
		isReadyToSit = false;
		isSitting = false;
	}
}

void CMario::RenderMarioSit(int& ani) {
	if (isSitting) {
		if (nx > 0) ani = MARIO_ANI_BIG_SITTING_RIGHT;
		else ani = MARIO_ANI_BIG_SITTING_LEFT;
	}
}

void CMario::HandleMarioKicking() {
	if (isKicking) {
		if (GetTickCount64() - start_kicking > MARIO_KICKING_TIME) {
			StopKicking();
			//SetState(MARIO_STATE_IDLE);
		}
	}
}

void CMario::HandleChangeYTransform() {
	if (state == MARIO_STATE_TRANSFORM) {
		if (level == MARIO_LEVEL_SMALL && !isChangingY) {
			y -= MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT;
		}
		if (level == MARIO_LEVEL_BIG && !isChangingY) {
			y -= MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT;
		}
		if (level == MARIO_LEVEL_TAIL && !isChangingY) {
			y -= MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT;
		}
		isChangingY = true;
	}
}

void CMario::HandleMarioHolding() {
	if (!isReadyToHold) isHolding = false;
}

void CMario::HandleBasicMarioDie() {
	x += dx;
	y += dy;
	if (untouchable != 1) {
		if (level == MARIO_LEVEL_BIG) {
			SetLevel(MARIO_LEVEL_SMALL);
			StartUntouchable();
		}
		else if (level == MARIO_LEVEL_SMALL) SetState(MARIO_STATE_DIE);
	}
}