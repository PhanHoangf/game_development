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
#include "Leaf.h"
#include "Point.h"
#include "PlayScence.h"

CMario::CMario(float x, float y) : CGameObject()
{
	//level = MARIO_LEVEL_BIG;
	level = MARIO_LEVEL_SMALL;
	//level = MARIO_LEVEL_TAIL;
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
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();

	//DebugOut(L"mario->x: %f", x);

	// Simple fall down
	vy += ay * dt;
	vx += ax * dt;
	//DebugOut(L"vy::%f\n", vy);

	limitMarioSpeed(vx, nx);
	handleMarioJump();
	HandleFlapping();

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
	HandleTurning();
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
						if (level == MARIO_LEVEL_TAIL) {
							SetLevel(MARIO_LEVEL_BIG);
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
				if (e->nx != 0 || e->ny > 0) {
					vy = 0;
					if (koopas->GetState() == KOOPAS_STATE_WALKING || koopas->GetState() == KOOPAS_STATE_SPINNING) {
						if (untouchable != 1) {
							if (level == MARIO_LEVEL_SMALL) SetState(MARIO_STATE_DIE);
							else if (level == MARIO_LEVEL_BIG) {
								StartUntouchable();
								SetLevel(MARIO_LEVEL_SMALL);
							}
							else if (level == MARIO_LEVEL_TAIL) {
								StartUntouchable();
								SetLevel(MARIO_LEVEL_BIG);
							}
						}
						else {
							vy = 0;
							x += dx;

						}
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
					CGame* game = CGame::GetInstance();

					vy = -MARIO_JUMP_DEFLECT_SPEED;
					if (koopas->GetState() == KOOPAS_STATE_WALKING) {
						if (koopas->tag == KOOPAS_GREEN_PARA) {
							koopas->SetTag(KOOPAS_GREEN);
						}
						else koopas->SetState(KOOPAS_STATE_IN_SHELL);
					}
					else if (koopas->GetState() == KOOPAS_STATE_IN_SHELL) {
						koopas->SetState(KOOPAS_STATE_SPINNING);
					}
					else if (koopas->GetState() == KOOPAS_STATE_SPINNING) {
						koopas->SetState(KOOPAS_STATE_IN_SHELL);
					}
					AddScore(this->x, this->y, 100);
				}
			}
			if (dynamic_cast<Coin*>(e->obj)) {
				Coin* coin = dynamic_cast<Coin*>(e->obj);
				if (e->ny != 0 || e->nx != 0) {
					coin->SetAppear(false);
					coin->SetIsDestroyed(true);
					AddScore(this->x, this->y, 100);
					AddCoin();
				}
			}
			if (dynamic_cast<Leaf*>(e->obj)) {
				Leaf* leaf = dynamic_cast<Leaf*>(e->obj);
				if (e->ny != 0 || e->nx != 0) {
					if (level != MARIO_LEVEL_TAIL) StartTransform(MARIO_LEVEL_TAIL);
					leaf->SetAppear(false);
					leaf->SetIsDestroyed(true);
					leaf->vy = 50.0f;
					AddScore(this->x, this->y, 1000);
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
	int alpha = 255;
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
		if (level == MARIO_LEVEL_TAIL) ani = MARIO_ANI_TRANSFORM_BANG;
	}

	if (untouchable) alpha = 128;
	if (state == MARIO_STATE_TRANSFORM) {
		animation_set->at(ani)->Render(x, y, alpha);
		/*if (level == MARIO_LEVEL_SMALL) animation_set->at(ani)->Render(x, y, alpha);
		else if (level == MARIO_LEVEL_BIG) animation_set->at(ani)->Render(x, y, alpha);
		else if (level == MARIO_LEVEL_TAIL) animation_set->at(ani)->Render(x, y, alpha);*/
	}
	/*if (state == MARIO_STATE_SITDOWN) {
		animation_set->at(ani)->Render(x, y, alpha);
	}*/
	if (level == MARIO_LEVEL_TAIL) {
		RenderMarioAniTail(ani);
		if (state == MARIO_STATE_TAIL_ATTACK)
		{
			DebugOut(L"[TURNING_STACK]::%d\n", turningStack);
			if (isTuring && nx > 0) {
				if (turningStack == 1 || turningStack == 5) CSprites::GetInstance()->Get(MARIO_SPRITE_WHACK_RIGHT_1_ID)->Draw(x, y, alpha);
				if (turningStack == 2) CSprites::GetInstance()->Get(MARIO_SPRITE_WHACK_RIGHT_2_ID)->Draw(x, y, alpha);
				if (turningStack == 3) CSprites::GetInstance()->Get(MARIO_SPRITE_WHACK_RIGHT_3_ID)->Draw(x, y, alpha);
				if (turningStack == 4) CSprites::GetInstance()->Get(MARIO_SPRITE_WHACK_RIGHT_4_ID)->Draw(x, y, alpha);
			}
			if (isTuring && nx < 0) {
				if (turningStack == 1 || turningStack == 5) CSprites::GetInstance()->Get(MARIO_SPRITE_WHACK_LEFT_1_ID)->Draw(x, y, alpha);
				if (turningStack == 2) CSprites::GetInstance()->Get(MARIO_SPRITE_WHACK_LEFT_2_ID)->Draw(x, y, alpha);
				if (turningStack == 3) CSprites::GetInstance()->Get(MARIO_SPRITE_WHACK_LEFT_3_ID)->Draw(x, y, alpha);
				if (turningStack == 4) CSprites::GetInstance()->Get(MARIO_SPRITE_WHACK_LEFT_4_ID)->Draw(x, y, alpha);
			}
		}
		else animation_set->at(ani)->Render(nx > 0 ? x - 6 : x, y, alpha);
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
				ani = MARIO_ANI_TAIL_HOLD_IDLE_RIGHT;
			}
			if (nx < 0) {
				ani = MARIO_ANI_TAIL_HOLD_IDLE_LEFT;
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
			ani = MARIO_ANI_TAIL_BRAKING_LEFT;
		}
		if (!isChangeDirection && isOnGround || abs(vx) < MARIO_WALKING_SPEED_MIN && isOnGround) {
			ani = MARIO_ANI_TAIL_WALKING_RIGHT;
		}
		if (!isOnGround) {
			if (this->isFlapping)
				ani = MARIO_ANI_TAIL_FLAPPING_RIGHT;
			else ani = MARIO_ANI_TAIL_JUMPINGUP_RIGHT;
		}
		if (isKicking) {
			ani = MARIO_ANI_TAIL_KICKING_RIGHT;
		}
		if (isHolding) {
			ani = MARIO_ANI_BIG_HOLD_WALKING_RIGHT;
		}
	}
	if (state == MARIO_STATE_WALKING_LEFT) {
		if (isChangeDirection && abs(vx) > MARIO_WALKING_SPEED_MIN) {
			ani = MARIO_ANI_TAIL_BRAKING_RIGHT;
		}
		if (!isChangeDirection && isOnGround || abs(vx) < MARIO_WALKING_SPEED_MIN && isOnGround) {
			ani = MARIO_ANI_TAIL_WALKING_LEFT;
		}
		if (!isOnGround) {
			if (isFlapping)
				ani = MARIO_ANI_TAIL_FLAPPING_LEFT;
			else ani = MARIO_ANI_TAIL_JUMPINGUP_LEFT;
		}
		if (isKicking) {
			ani = MARIO_ANI_TAIL_KICKING_LEFT;
		}
		if (isHolding) {
			ani = MARIO_ANI_BIG_HOLD_WALKING_LEFT;
		}
	}
	if (state == MARIO_STATE_JUMP) {
		if (nx > 0) {
			int res = isFlapping ? 1 : 0;
			DebugOut(L"Render flapping::%d\n", res);
			if (isFlapping)
				ani = MARIO_ANI_TAIL_FLAPPING_RIGHT;
			else
				ani = MARIO_ANI_TAIL_JUMPINGUP_RIGHT;
		}
		if (nx < 0) {
			if (isFlapping) ani = MARIO_ANI_TAIL_FLAPPING_LEFT;
			else ani = MARIO_ANI_TAIL_JUMPINGUP_LEFT;
		}
	}
	if (state == MARIO_STATE_JUMP_X) {
		if (nx > 0) {
			ani = MARIO_ANI_TAIL_JUMPINGUP_RIGHT;
		}
		if (nx < 0) {
			ani = MARIO_ANI_TAIL_JUMPINGUP_LEFT;
		}
	}
	if (state == MARIO_STATE_SITDOWN) {
		RenderMarioSit(ani);
	}
	if (state == MARIO_STATE_KICK) {
		if (isKicking)
		{
			if (nx > 0) ani = MARIO_ANI_TAIL_KICKING_RIGHT;
			if (nx < 0) ani = MARIO_ANI_TAIL_KICKING_LEFT;
		}
	}
	if (state == MARIO_STATE_HOLD) {
		if (isHolding) {
			if (nx > 0) ani = MARIO_ANI_BIG_HOLD_WALKING_RIGHT;
			if (nx < 0) ani = MARIO_ANI_BIG_HOLD_WALKING_LEFT;
		}
	}
	if (state == MARIO_STATE_TAIL_ATTACK) {
		if (nx > 0) ani = MARIO_ANI_TAIL_TURNING_RIGHT;
		if (nx < 0) ani = MARIO_ANI_TAIL_TURNING_LEFT;
	}
}

void CMario::SetState(int state)
{
	/*DebugOut(L"state::%d\n", state);*/
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
	case MARIO_STATE_TAIL_ATTACK:
		if (!isTuring) {
			turningStack = 0;
			//isTuring = true;
			StartTurning();
		}
		if (previousState == MARIO_STATE_SITDOWN)
			y -= MARIO_BIG_BBOX_HEIGHT - MARIO_BBOX_SIT_HEIGHT;
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

void CMario::HandleChangeXTail() {
	if (level == MARIO_LEVEL_TAIL) {
		if (!isChagingX) {
			x += 5;
			isChagingX = true;
		}
	}
	else isChagingX = false;
}

void CMario::RenderMarioSit(int& ani) {
	if (isSitting) {
		if (level == MARIO_LEVEL_TAIL) {
			if (nx > 0) ani = MARIO_ANI_TAIL_SITTING_RIGHT;
			else ani = MARIO_ANI_TAIL_SITTING_LEFT;
		}
		else {
			if (nx > 0) ani = MARIO_ANI_BIG_SITTING_RIGHT;
			else ani = MARIO_ANI_BIG_SITTING_LEFT;
		}
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
			y -= MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT + 3;
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
	//y += dy;
	if (untouchable != 1) {
		if (level == MARIO_LEVEL_TAIL) {
			SetLevel(MARIO_LEVEL_BIG);
			StartUntouchable();
			return;
		}
		if (level == MARIO_LEVEL_BIG) {
			SetLevel(MARIO_LEVEL_SMALL);
			StartUntouchable();
			return;
		}
		else if (level == MARIO_LEVEL_SMALL) SetState(MARIO_STATE_DIE);
	}
}

void CMario::HandleTurning() {

	if (GetTickCount64() - start_turning >= MARIO_TURING_TIME && isTuring) {
		start_turning = GetTickCount64();
		turningStack++;
	}
	if (GetTickCount64() - start_turning_state > MARIO_TURNING_TOTAL_TIME && isTuring) {
		isTuring = false;
		start_turning_state = 0;
		start_turning = 0;
		turningStack = 0;
	}

}

void CMario::AddScore(float x, float y, int score) {
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	Point* point = new Point(score);
	point->SetPosition(x, y);
	currentScene->AddSpecialObject(point);
	this->marioScore += score;
}

void CMario::HandleFlapping() {
	if (level == MARIO_LEVEL_TAIL && isFlapping) {
		vy = MARIO_SLOW_FALLING_SPEED;
	}
}