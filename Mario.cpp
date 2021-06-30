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
#include "BreakableBrick.h"
#include "FireFlower.h"
#include "MarioBullet.h"
#include "IntroGround.h"

CMario::CMario(float x, float y) : CGameObject()
{
	//level = MARIO_LEVEL_BIG;
	//level = MARIO_LEVEL_SMALL;
	level = MARIO_LEVEL_TAIL;
	//level = MARIO_LEVEL_FIRE;
	untouchable = 0;
	ax = MARIO_ACCELERATION;
	ay = MARIO_ACCELERATION_JUMP;
	SetState(MARIO_STATE_IDLE);
	SetDirection(MARIO_DIRECT_RIGHT);
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
	this->tail = new Tail(80, y);
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();

	DebugOut(L"mario->y: %f\n", y);

	// Simple fall down
	if (!isJumpOnMusicBrick)
	{
		vy += ay * dt;
	}
	vx += ax * dt;
	//DebugOut(L"vy::%f\n", vy);
	if (isHolding) {
		DebugOut(L"vx::%f\n", vx);
	}
	limitMarioSpeed(vx, nx);
	if (!isFlying && !isTailFlying)
		handleMarioJump();
	HandleFlapping();

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	if (vy < 0)
		isOnGround = false;

	if (isOnGround) {
		isFlying = false;
		isTailFlying = false;
	}

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
	HandleSpeedStack();
	HandleFlying();
	HandleShooting();

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
		isAttacked = false;
	}

	if (GetTickCount64() - start_score_time >= STACK_SCORE_TIME && isStackingScore) {
		isStackingScore = false;
		stackScoreTimes = 0;
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

		//how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
	/*	if (rdx != 0 && rdx != dx)
			x += nx * abs(rdx);*/
			// block every object first!

		x0 = x;
		y0 = y;

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
			if (state == MARIO_STATE_DIE || isSwitchScene) return;
			LPCOLLISIONEVENT e = coEventsResult[i];
			GetBoundingBox(mLeft, mTop, mRight, mBottom);
			e->obj->GetBoundingBox(oLeft, oTop, oRight, oBottom);
			if (dynamic_cast<IntroGround*>(e->obj)) {

			}
			if (dynamic_cast<CBrick*>(e->obj) || dynamic_cast<QuestionBrick*>(e->obj) || dynamic_cast<IntroGround*>(e->obj)) {
				CBrick* brick = dynamic_cast<CBrick*>(e->obj);
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
				if (e->nx < 0 && brick != NULL && ceil(mBottom) != oTop) {
					vx = 0;
					if (brick->GetType() == PUSH_TO_RIGHT)
						brick->SetState(BRICK_STATE_HIT);
				}
				if (e->ny > 0 && brick != NULL) {
					vy = 0;
					ay = MARIO_GRAVITY;
					if (brick->GetType() == PUSH_UP)
						brick->SetState(BRICK_STATE_HIT);
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
				if (e->nx > 0 || e->nx < 0) {
					HandleBasicMarioDie();
				}
			} // if Goomba
			if (dynamic_cast<BoomerangKoopas*>(e->obj)) // if e->obj is Goomba 
			{
				BoomerangKoopas* bKoopas = dynamic_cast<BoomerangKoopas*>(e->obj);
				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (bKoopas->GetState() != BOOMERANG_KOOPAS_STATE_DIE)
					{
						bKoopas->SetState(BOOMERANG_KOOPAS_STATE_DIE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
						AddScore(x, y, 100);
					}
				}
				//if (e->nx != 0) {
				//	if (untouchable != 1) {
				//		if (level == MARIO_LEVEL_SMALL) {
				//			SetState(MARIO_STATE_DIE);
				//		}
				//		//! 1: Set back to level small -> 2: start untouchable
				//		if (level == MARIO_LEVEL_BIG) {
				//			SetLevel(MARIO_LEVEL_SMALL);
				//			StartUntouchable();
				//		}
				//		if (level == MARIO_LEVEL_TAIL) {
				//			SetLevel(MARIO_LEVEL_BIG);
				//			StartUntouchable();
				//		}
				//	}
				//	else {
				//		x += dx;
				//	}
				//}
			} // if Goomba
			if (dynamic_cast<CKoopas*>(e->obj)) {
				CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
				if (e->nx != 0) {
					if (koopas->GetState() == KOOPAS_STATE_IN_SHELL || koopas->GetState() == KOOPAS_STATE_SHELL_UP) {
						if (isReadyToHold) {
							isHolding = true;
							koopas->SetIsBeingHeld(true);
						}
						else {
							SetState(MARIO_STATE_KICK);
							koopas->SetState(KOOPAS_STATE_SPINNING);
						}
					}
					else {
						HandleBasicMarioDie();
					}
				}
				if (e->ny > 0) {
					if (koopas->GetState() == KOOPAS_STATE_IN_SHELL || koopas->GetState() == KOOPAS_STATE_SHELL_UP) {
						SetState(MARIO_STATE_KICK);
						koopas->SetState(KOOPAS_STATE_SPINNING);
					}
					else {
						koopas->x = this->x + nx * 2;
						HandleBasicMarioDie();
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
			if (dynamic_cast<FireFlower*>(e->obj)) {
				FireFlower* fireFlower = dynamic_cast<FireFlower*>(e->obj);
				if (e->ny != 0 || e->nx != 0) {
					if (level != MARIO_LEVEL_FIRE) StartTransform(MARIO_LEVEL_FIRE);
					fireFlower->isAppear = false;
					fireFlower->SetIsDestroyed(true);
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
						AddScore(x, y, 100);
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
			if (dynamic_cast<CardItem*>(e->obj)) {
				CardItem* card = dynamic_cast<CardItem*>(e->obj);
				if (e->ny != 0 || e->nx != 0) {
					card->SetAppear(false);
					card->isDestroyed = true;
					AddCard(card->state - 1);
				}
			}
			if (dynamic_cast<MusicBrick*>(e->obj)) {
				MusicBrick* msBrick = dynamic_cast<MusicBrick*>(e->obj);
				vx = 0;
				if (e->nx != 0) {
					vx = 0;
				}
				if (e->ny < 0) {
					if (msBrick->GetState() == MUSIC_BRICK_STATE_IDLE && !msBrick->GetIsGoUp()) {
						msBrick->SetState(MUSIC_BRICK_STATE_DOWN);
						msBrick->SetIsPushedDown(true);
					}
				}
				if (e->ny > 0) {
					msBrick->SetState(MUSIC_BRICK_STATE_UP);
					msBrick->SetIsPushedUp(true);
					vy = 0;
					ay = MARIO_GRAVITY;
				}
			}
			if (dynamic_cast<Boomerang*>(e->obj)) {
				if (e->ny != 0 || e->nx != 0) {
					y = y0 + dy;
					x = x0 + dx;
				}
			}
			if (dynamic_cast<MarioBullet*>(e->obj)) {
				x = x0 + dx;
				if (e->ny > 0) {
					y = y0 + dx;
				}
				else y = y0;
			}
			if (dynamic_cast<CMario*>(e->obj)) {
				CMario* tmp = dynamic_cast<CMario*>(e->obj);
				x = x0;
				y = y0;
				if (e->ny < 0)
				{
					vy = -0.7f;
					//isDeflect = true;
				}
				else
				{
					SetState(MARIO_STATE_SITDOWN);
					vx = vy = 0;
				}
			}
			else if (dynamic_cast<CPortal*>(e->obj))
			{
				DebugOut(L"portal");
				isSwitchScene = true;
				CPortal* p = dynamic_cast<CPortal*>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
		}
	}

	float cy = CGame::GetInstance()->GetCamPosY();
	float cx = CGame::GetInstance()->GetCamPosX();

	//! Limit y when fly
	if (y <= cy) {
		if (isFlying || isTailFlying) {
			y = cy + 16;
		}
	}

	if (x <= cx) {
		x = cx;
	}
	tail->Update(dt, coObjects);
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CMario::limitMarioSpeed(float& vx, int nx) {
	if (abs(vx) > MARIO_SPEED_MAX && !isReadyToRun) {
		vx = nx * MARIO_SPEED_MAX;
	}
	if (abs(vx) > MARIO_RUNNING_SPEED_MAX && isReadyToRun) {
		vx = nx * MARIO_RUNNING_SPEED_MAX;
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
	if (level == MARIO_LEVEL_FIRE) {
		RenderMarioAniFire(ani);
	}

	if (state == MARIO_STATE_TRANSFORM) {
		if (nx > 0) {
			ani = MARIO_ANI_TRANSFORM_SMALL_RIGHT;
		}
		else ani = MARIO_ANI_TRANSFORM_SMALL_LEFT;
		if (level == MARIO_LEVEL_TAIL || isBangAni || level == MARIO_LEVEL_FIRE) {
			ani = MARIO_ANI_TRANSFORM_BANG;
		}

	}

	if (untouchable) alpha = 128;
	if (state == MARIO_STATE_TRANSFORM) {
		animation_set->at(ani)->Render(x, y, alpha);
	}
	if (level == MARIO_LEVEL_TAIL) {
		RenderMarioAniTail(ani);
		if (state == MARIO_STATE_TAIL_ATTACK)
		{
			DebugOut(L"[TURNING_STACK]::%d\n", turningStack);
			if (isTuring && nx > 0) {
				if (turningStack == 1 || turningStack == 5) CSprites::GetInstance()->Get(MARIO_SPRITE_WHACK_RIGHT_1_ID)->Draw(x - 8, y, alpha);
				if (turningStack == 2) CSprites::GetInstance()->Get(MARIO_SPRITE_WHACK_RIGHT_2_ID)->Draw(x, y, alpha);
				if (turningStack == 3) CSprites::GetInstance()->Get(MARIO_SPRITE_WHACK_RIGHT_3_ID)->Draw(x, y, alpha);
				if (turningStack == 4) CSprites::GetInstance()->Get(MARIO_SPRITE_WHACK_RIGHT_4_ID)->Draw(x, y, alpha);
			}
			if (isTuring && nx < 0) {
				if (turningStack == 1 || turningStack == 5) CSprites::GetInstance()->Get(MARIO_SPRITE_WHACK_LEFT_1_ID)->Draw(x, y, alpha);
				if (turningStack == 2) CSprites::GetInstance()->Get(MARIO_SPRITE_WHACK_LEFT_2_ID)->Draw(x, y, alpha);
				if (turningStack == 3) CSprites::GetInstance()->Get(MARIO_SPRITE_WHACK_LEFT_3_ID)->Draw(x - 8, y, alpha);
				if (turningStack == 4)
					CSprites::GetInstance()->Get(MARIO_SPRITE_WHACK_LEFT_4_ID)->Draw(x, y, alpha);
			}
		}
		else animation_set->at(ani)->Render(nx > 0 ? x - 6 : x, y, alpha);
		this->tail->Render();
		//if (isTuring)
			//this->tail->Render();
	}
	else animation_set->at(ani)->Render(x, y, alpha);

	RenderBoundingBox();
}

void CMario::RenderMarioAniSmall(int& ani) {
	if (state == MARIO_STATE_IDLE) {
		if (isKicking) {
			if (nx > 0) {
				ani = MARIO_ANI_SMALL_KICKING_RIGHT;
			}
			if (nx < 0) {
				ani = MARIO_ANI_SMALL_KICKING_LEFT;
			}
		}
		else if (isHolding) {
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
			if (isFlying) {
				ani = MARIO_ANI_SMALL_FLY_RIGHT;
			}
			if (isHolding) {
				ani = MARIO_ANI_SMALL_HOLD_JUMPING_RIGHT;
			}
		}
		if (isKicking) {
			ani = MARIO_ANI_SMALL_KICKING_RIGHT;
		}
		if (isHolding) {
			ani = MARIO_ANI_SMALL_HOLD_WALKING_RIGHT;
		}
		if (isRunning && isOnGround && !isHolding) {
			if (vx > MARIO_SPEED_MAX && vx < MARIO_RUNNING_SPEED_MAX) {
				ani = MARIO_SMALL_WALK_FAST_RIGHT;
			}
			if (vx >= MARIO_RUNNING_SPEED_MAX) {
				ani = MARIO_MAX_SPEED_SMALL_RIGHT;
			}
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
			if (isFlying) {
				ani = MARIO_ANI_SMALL_FLY_LEFT;
			}
			if (isHolding) {
				ani = MARIO_ANI_SMALL_HOLD_JUMPING_LEFT;
			}
		}
		if (isKicking) {
			ani = MARIO_ANI_SMALL_KICKING_LEFT;
		}
		if (isHolding) {
			ani = MARIO_ANI_SMALL_HOLD_WALKING_LEFT;
		}
		if (isRunning && isOnGround && !isHolding) {
			if (abs(vx) > MARIO_SPEED_MAX && vx < MARIO_RUNNING_SPEED_MAX) {
				ani = MARIO_SMALL_WALK_FAST_LEFT;
			}
			if (abs(vx) >= MARIO_RUNNING_SPEED_MAX) {
				ani = MARIO_MAX_SPEED_SMALL_LEFT;
			}
		}
	}
	if (state == MARIO_STATE_JUMP) {
		if (nx > 0) {
			ani = MARIO_JUMP_SMALL_RIGHT;
			if (isFlying) {
				ani = MARIO_ANI_SMALL_FLY_RIGHT;
			}
		}
		if (nx < 0) {
			ani = MARIO_JUMP_SMALL_LEFT;
			if (isFlying) {
				ani = MARIO_ANI_SMALL_FLY_LEFT;
			}
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
		if (isKicking) {
			if (nx > 0) {
				ani = MARIO_ANI_BIG_KICKING_RIGHT;
			}
			if (nx < 0) {
				ani = MARIO_ANI_BIG_KICKING_LEFT;
			}
		}
		else if (isHolding) {
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
			if (isFlying) {
				ani = MARIO_ANI_BIG_FLY_RIGHT;
			}
			if (isHolding)
				ani = MARIO_ANI_BIG_HOLD_JUMPING_RIGHT;
		}
		if (isKicking) {
			ani = MARIO_ANI_BIG_KICKING_RIGHT;
		}
		if (isHolding) {
			ani = MARIO_ANI_BIG_HOLD_WALKING_RIGHT;
		}
		if (isRunning && isOnGround) {
			if (abs(vx) >= MARIO_RUNNING_SPEED_MAX && !isHolding) {
				ani = MARIO_ANI_BIG_WALKING_FAST_RIGHT;
			}
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
			if (isFlying) {
				ani = MARIO_ANI_BIG_FLY_LEFT;
			}
			if (isHolding)
				ani = MARIO_ANI_BIG_HOLD_JUMPING_LEFT;
		}
		if (isKicking) {
			ani = MARIO_ANI_BIG_KICKING_LEFT;
		}
		if (isHolding) {
			ani = MARIO_ANI_BIG_HOLD_WALKING_LEFT;
		}
		if (isRunning && isOnGround) {
			if (abs(vx) >= MARIO_RUNNING_SPEED_MAX && !isHolding) {
				ani = MARIO_ANI_BIG_WALKING_FAST_LEFT;
			}
		}
	}
	if (state == MARIO_STATE_JUMP) {
		if (nx > 0) {
			ani = MARIO_ANI_BIG_JUMPINGUP_RIGHT;
			if (isFlying)
				ani = MARIO_ANI_BIG_FLY_RIGHT;
			if (isHolding)
				ani = MARIO_ANI_BIG_HOLD_JUMPING_RIGHT;
		}
		if (nx < 0) {
			ani = MARIO_ANI_BIG_JUMPINGUP_LEFT;
			if (isFlying)
				ani = MARIO_ANI_BIG_FLY_LEFT;
			if (isHolding)
				ani = MARIO_ANI_BIG_HOLD_JUMPING_LEFT;
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
		if (isKicking) {
			if (nx > 0) {
				ani = MARIO_ANI_TAIL_KICKING_RIGHT;
			}
			if (nx < 0) {
				ani = MARIO_ANI_TAIL_KICKING_LEFT;
			}
		}
		else if (isHolding) {
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
			if (isFlapping)
				ani = MARIO_ANI_TAIL_FLAPPING_RIGHT;
			else if (isTailFlying) {
				ani = MARIO_ANI_TAIL_FLY_UP_RIGHT;
			}
			else if (isHolding) {
				ani = MARIO_ANI_TAIL_HOLD_JUMPINGUP_RIGHT;
			}
			else ani = MARIO_ANI_TAIL_JUMPINGUP_RIGHT;
		}
		if (isKicking) {
			ani = MARIO_ANI_TAIL_KICKING_RIGHT;
		}
		if (isHolding) {
			ani = MARIO_ANI_TAIL_HOLD_WALKING_RIGHT;
		}
		if (isRunning && isOnGround && !isHolding) {
			if (abs(vx) >= MARIO_SPEED_MAX) {
				ani = MARIO_ANI_TAIL_WALKING_FAST_RIGHT;
			}
			if (abs(vx) >= MARIO_RUNNING_SPEED_MAX) {
				ani = MARIO_ANI_TAIL_RUNNING_RIGHT;
			}
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
			else if (isTailFlying) {
				ani = MARIO_ANI_TAIL_FLY_UP_LEFT;
			}
			else if (isHolding) {
				ani = MARIO_ANI_TAIL_HOLD_JUMPINGUP_LEFT;
			}
			else ani = MARIO_ANI_TAIL_JUMPINGUP_LEFT;
		}
		if (isKicking) {
			ani = MARIO_ANI_TAIL_KICKING_LEFT;
		}
		if (isHolding) {
			ani = MARIO_ANI_TAIL_HOLD_WALKING_LEFT;
		}
		if (isRunning && isOnGround && !isHolding) {
			if (abs(vx) >= MARIO_SPEED_MAX) {
				ani = MARIO_ANI_TAIL_WALKING_FAST_LEFT;
			}
			if (abs(vx) >= MARIO_RUNNING_SPEED_MAX) {
				ani = MARIO_ANI_TAIL_RUNNING_LEFT;
			}
		}
	}
	if (state == MARIO_STATE_JUMP) {
		if (nx > 0) {
			if (isFlapping)
				ani = MARIO_ANI_TAIL_FLAPPING_RIGHT;
			else if (isTailFlying)
				ani = MARIO_ANI_TAIL_FLY_UP_RIGHT;
			else if (isHolding)
				ani = MARIO_ANI_TAIL_HOLD_JUMPINGUP_RIGHT;
			else
				ani = MARIO_ANI_TAIL_JUMPINGUP_RIGHT;
		}
		if (nx < 0) {
			if (isFlapping)
				ani = MARIO_ANI_TAIL_FLAPPING_LEFT;
			else if (isTailFlying)
				ani = MARIO_ANI_TAIL_FLY_UP_LEFT;
			else if (isHolding)
				ani = MARIO_ANI_TAIL_HOLD_JUMPINGUP_LEFT;
			else
				ani = MARIO_ANI_TAIL_JUMPINGUP_LEFT;
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

void CMario::RenderMarioAniFire(int& ani) {
	if (state == MARIO_STATE_IDLE) {
		//DebugOut(L"hold::%d\n", hold);
		if (isKicking) {
			if (nx > 0) {
				ani = MARIO_ANI_FIRE_KICKING_RIGHT;
			}
			if (nx < 0) {
				ani = MARIO_ANI_FIRE_KICKING_LEFT;
			}
		}
		else if (isHolding) {
			if (nx > 0) {
				ani = MARIO_ANI_FIRE_HOLD_IDLE_RIGHT;
			}
			if (nx < 0) {
				ani = MARIO_ANI_FIRE_HOLD_IDLE_LEFT;
			}
		}
		else if (isShooting) {
			if (nx > 0) {
				ani = MARIO_ANI_SHOOTING_RIGHT;
			}
			if (nx < 0) {
				ani = MARIO_ANI_SHOOTING_LEFT;
			}
		}
		else {
			if (nx > 0) {
				ani = MARIO_ANI_FIRE_IDLE_RIGHT;
			}
			if (nx < 0) {
				ani = MARIO_ANI_FIRE_IDLE_LEFT;
			}
		}
	}
	if (state == MARIO_STATE_WALKING_RIGHT) {
		if (isChangeDirection && abs(vx) > MARIO_WALKING_SPEED_MIN) {
			ani = MARIO_ANI_FIRE_BRAKING_LEFT;
		}
		if (!isChangeDirection && isOnGround || abs(vx) < MARIO_WALKING_SPEED_MIN && isOnGround) {
			ani = MARIO_ANI_FIRE_WALKING_RIGHT;
		}
		if (!isOnGround) {
			ani = MARIO_ANI_FIRE_JUMPINGUP_RIGHT;
			if (isFlying) {
				ani = MARIO_ANI_FIRE_FLY_RIGHT;
			}
			if (isHolding)
				ani = MARIO_ANI_FIRE_HOLD_JUMPINGUP_RIGHT;
		}
		if (isKicking) {
			ani = MARIO_ANI_FIRE_KICKING_RIGHT;
		}
		if (isHolding) {
			ani = MARIO_ANI_FIRE_HOLD_WALKING_RIGHT;
		}
		if (isShooting) {
			ani = MARIO_ANI_SHOOTING_RIGHT;
		}
		if (isRunning && isOnGround) {
			if (abs(vx) >= MARIO_RUNNING_SPEED_MAX && !isHolding) {
				ani = MARIO_ANI_FIRE_WALKING_FAST_RIGHT;
			}
		}
	}
	if (state == MARIO_STATE_WALKING_LEFT) {
		if (isChangeDirection && abs(vx) > MARIO_WALKING_SPEED_MIN) {
			ani = MARIO_ANI_FIRE_BRAKING_RIGHT;
		}
		if (!isChangeDirection && isOnGround || abs(vx) < MARIO_WALKING_SPEED_MIN && isOnGround) {
			ani = MARIO_ANI_FIRE_WALKING_LEFT;
		}
		if (!isOnGround) {
			ani = MARIO_ANI_FIRE_JUMPINGUP_LEFT;
			if (isFlying) {
				ani = MARIO_ANI_FIRE_FLY_LEFT;
			}
			if (isHolding)
				ani = MARIO_ANI_FIRE_HOLD_JUMPINGUP_LEFT;
		}
		if (isKicking) {
			ani = MARIO_ANI_FIRE_KICKING_LEFT;
		}
		if (isHolding) {
			ani = MARIO_ANI_FIRE_HOLD_WALKING_LEFT;
		}
		if (isShooting) {
			ani = MARIO_ANI_SHOOTING_LEFT;
		}
		if (isRunning && isOnGround) {
			if (abs(vx) >= MARIO_RUNNING_SPEED_MAX && !isHolding) {
				ani = MARIO_ANI_FIRE_WALKING_FAST_LEFT;
			}
		}
	}
	if (state == MARIO_STATE_JUMP) {
		if (nx > 0) {
			ani = MARIO_ANI_FIRE_JUMPINGUP_RIGHT;
			if (isFlying)
				ani = MARIO_ANI_FIRE_FLY_RIGHT;
			if (isHolding)
				ani = MARIO_ANI_BIG_HOLD_JUMPING_RIGHT;
			if (isShooting) {
				ani = MARIO_ANI_SHOOTING_RIGHT;
			}
		}
		if (nx < 0) {
			ani = MARIO_ANI_FIRE_JUMPINGUP_LEFT;
			if (isFlying)
				ani = MARIO_ANI_FIRE_FLY_LEFT;
			if (isHolding)
				ani = MARIO_ANI_FIRE_HOLD_JUMPINGUP_LEFT;
			if (isShooting) {
				ani = MARIO_ANI_SHOOTING_LEFT;
			}
		}
	}
	if (state == MARIO_STATE_JUMP_X) {
		if (nx > 0) {
			ani = MARIO_ANI_FIRE_JUMPINGUP_RIGHT;
		}
		if (nx < 0) {
			ani = MARIO_ANI_FIRE_JUMPINGUP_LEFT;
		}
	}
	if (state == MARIO_STATE_SITDOWN) {
		RenderMarioSit(ani);
	}
	if (state == MARIO_STATE_KICK) {
		if (isKicking)
		{
			if (nx > 0) ani = MARIO_ANI_FIRE_KICKING_RIGHT;
			if (nx < 0) ani = MARIO_ANI_FIRE_KICKING_LEFT;
		}
	}
	/*if (state == MARIO_STATE_HOLD) {
		if (isHolding) {
			if (nx > 0) ani = MARIO_ANI_BIG_HOLD_WALKING_RIGHT;
			if (nx < 0) ani = MARIO_ANI_BIG_HOLD_WALKING_LEFT;
		}
	}*/
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
		isRunning = false;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		if (ax < 0 && isOnGround && abs(vx) > MARIO_WALKING_SPEED_START) {
			isChangeDirection = true;
			runningStack = 0;
		}
		else {
			runningStack++;
		}
		if (vx > MARIO_SPEED_MAX && isReadyToRun) {
			isRunning = true;
		}
		else {
			isRunning = false;
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
		if (abs(vx) > MARIO_SPEED_MAX && isReadyToRun) {
			isRunning = true;
		}
		else {
			isRunning = false;
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
		if (speedStack == MARIO_SPEED_STACK) {
			if (level == MARIO_LEVEL_TAIL) {
				isTailFlying = true;
				StartTailFlying();
			}
			else {
				isFlying = true;
				StartFlying();
			}
			normalFlyPullDown = false;
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
	if (level != MARIO_LEVEL_SMALL)
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
		if (GetTickCount64() - start_transform > MARIO_TRANSFORM_TIME) {
			StopTransform();
			isBangAni = false;
			if (isAttacked) {
				StartUntouchable();
			}
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
		else if (level == MARIO_LEVEL_FIRE) {
			if (nx > 0) ani = MARIO_ANI_FIRE_SITTING_RIGHT;
			else ani = MARIO_ANI_FIRE_SITTING_LEFT;
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
			SetState(MARIO_STATE_IDLE);
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
	if (untouchable != 1) {
		isAttacked = true;
		x = x0;
		if (state == MARIO_STATE_SITDOWN)
			y -= MARIO_BIG_BBOX_HEIGHT - MARIO_BBOX_SIT_HEIGHT;
		else
			y = y0 + dy - 1;
		if (level == MARIO_LEVEL_TAIL) {
			StartTransform(MARIO_LEVEL_BIG);
			StartUntouchable();
			SetState(MARIO_STATE_IDLE);
		}
		else if (level == MARIO_LEVEL_FIRE) {
			StartTransform(MARIO_LEVEL_BIG);
			StartUntouchable();
			SetState(MARIO_STATE_IDLE);
		}
		else if (level == MARIO_LEVEL_BIG) {
			StartTransform(MARIO_LEVEL_SMALL);
			StartUntouchable();
			//y -= MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT;
			SetState(MARIO_STATE_IDLE);
		}
		else {
			SetState(MARIO_STATE_DIE);
		}
	}
	else {
		x = x0 + dx;
		y = y0;
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

void CMario::AddScore(float x, float y, int score, bool isStack) {
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();

	start_score_time = GetTickCount64();

	if (isStack) {
		if (!isStackingScore) {
			isStackingScore = true;
		}
		else {
			if (stackScoreTimes == 5) {
				score *= 10;
				stackScoreTimes = 5;
			}
			else {
				score = score * pow(2, stackScoreTimes);
			}
		}

		stackScoreTimes++;
	}

	Point* point = new Point(score);
	int previousScore = score;

	point->SetPosition(x, y);
	currentScene->AddSpecialObject(point);

	this->marioScore += score;

}

void CMario::HandleFlapping() {
	if (level == MARIO_LEVEL_TAIL && isFlapping) {
		vy = MARIO_SLOW_FALLING_SPEED;
	}
}
void CMario::HandleSpeedStack() {
	if (GetTickCount64() - start_running > MARIO_SPEED_STACK_TIME && isRunning && vx != 0 && isReadyToRun) {
		start_running = GetTickCount64();
		speedStack++;
		if (speedStack > MARIO_SPEED_STACK) {
			speedStack = MARIO_SPEED_STACK;
			isReadyToFly = true;
		}
	}
	if (GetTickCount64() - running_stop > MARIO_SPEED_STACK_LOST_TIME && !isRunning)
	{
		running_stop = GetTickCount64();
		speedStack--;
		isReadyToFly = false;
		if (speedStack < 0)
		{
			speedStack = 0;
			isRunning = false;
			isFlying = false;
		}
	}
}

void CMario::HandleFlying() {
	if (level == MARIO_LEVEL_SMALL || level == MARIO_LEVEL_BIG) {
		if (isFlying)
		{
			if (vy <= -MARIO_NORMAL_FLY_MAX) {
				normalFlyPullDown = true;
			}
		}
	}
	if (normalFlyPullDown && isFlying) {
		ay = MARIO_GRAVITY;
	}
	if (GetTickCount64() - fly_start > MARIO_FLYING_TIME && fly_start != 0 && isFlying)
	{
		fly_start = 0;
		isRunning = false;
		isFlying = false;
	}
	if (GetTickCount64() - tail_fly_start > MARIO_FLYING_TIME && tail_fly_start != 0 && isTailFlying)
	{
		tail_fly_start = 0;
		speedStack = 0;
		isRunning = false;
		isTailFlying = false;
	}
}


void CMario::ShootFireBall() {
	if (shootTimes < MARIO_MAX_BULLET) {
		CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		CAnimationSets* animation_sets = CAnimationSets::GetInstance();
		MarioBullet* bullet = new MarioBullet(this->nx);
		LPANIMATION_SET ani_set = animation_sets->Get(MARIO_BULLET_ANI_SET_ID);
		bullet->SetAnimationSet(ani_set);
		if (nx > 0) bullet->SetPosition(this->x + MARIO_BULLET_BBOX_WIDTH, this->y + 8);
		else  bullet->SetPosition(this->x, this->y + 8);
		bullet->isAppear = true;
		currentScene->AddSpecialObject(bullet);
		bullet->SetState(MARIO_BULLET_STATE_DOWN);
		StartShooting();
		shootTimes++;
	}
}

void CMario::HandleShooting() {
	if (isShooting && GetTickCount64() - start_shooting >= MARIO_SHOOTING_TIME) {
		StopShooting();
	}
}