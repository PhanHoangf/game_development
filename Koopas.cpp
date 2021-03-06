#include "Koopas.h"
#include "Block.h"
#include "Utils.h"
#include "Brick.h"
#include "Goomba.h"
#include "Mario.h"
#include "PlayScence.h"
#include "QuestionBrick.h"
#include "BreakableBrick.h"
#include "Coin.h"

CKoopas::CKoopas(int tag)
{
	this->start_x = x;
	this->start_y = y;
	this->start_tag = tag;
	if (tag == KOOPAS_GREEN || tag == KOOPAS_GREEN_PARA) {
		this->nx = -1;
	}
	this->SetState(KOOPAS_STATE_WALKING);
}

void CKoopas::Reset() {
	this->x = start_x;
	this->y = start_y;
	this->tag = start_tag;
	//this->isReviable = false;
	if (tag == KOOPAS_GREEN || tag == KOOPAS_GREEN_PARA) {
		this->nx = -1;
	}
	this->SetState(KOOPAS_STATE_WALKING);
}

void CKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;
	if (state == KOOPAS_STATE_IN_SHELL || state == KOOPAS_STATE_SPINNING || state == KOOPAS_STATE_SHELL_UP)
	{
		bottom = y + KOOPAS_BBOX_SHELL_HEIGHT;
	}
	else
		bottom = y + KOOPAS_BBOX_HEIGHT;
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = currentScene->GetPlayer();

	/*if (state == KOOPAS_STATE_INACTIVE && mario->x <= 1200.0f) return;
	if (state == KOOPAS_STATE_INACTIVE && mario->x >= 1200.0f) {
		SetState(KOOPAS_STATE_WALKING);
	}*/

	//CalRevivable();

	if (GetTickCount64() - shell_start >= KOOPAS_SHELL_TIME && shell_start != 0 && state != KOOPAS_STATE_SPINNING) {
		shell_start = 0;
		StartReviving();
	}

	if (GetTickCount64() - reviving_start >= KOOPAS_REVIVE_TIME && reviving_start != 0 && state != KOOPAS_STATE_SPINNING && shell_start == 0)
	{
		reviving_start = 0;
		y -= (KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_SHELL_HEIGHT) + 1.0f;
		/*	if (isBeingHeld)
			{
				isBeingHeld = false;
				mario->SetIsHolding(false);
			}*/
		SetState(KOOPAS_STATE_WALKING);
	}

	HandleBeingHeld(mario);
	//vx = 0;
	CGameObject::Update(dt, coObjects);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	vy += KOOPAS_GRAVITY * dt;

	coEvents.clear();

	if (state != KOOPAS_STATE_DEATH) {
		CalcPotentialCollisions(coObjects, coEvents);
	}
	float mLeft, mTop, mRight, mBottom;
	float oLeft, oTop, oRight, oBottom;
	if (mario != NULL) {
		if (mario->isTuring && mario->GetLevel() == MARIO_LEVEL_TAIL) {
			mario->GetTail()->GetBoundingBox(mLeft, mTop, mRight, mBottom);
			if (isColliding(floor(mLeft), floor(mTop), ceil(mRight), ceil(mBottom))) {
				SetState(KOOPAS_STATE_SHELL_UP);
				if (tag == KOOPAS_GREEN_PARA)
					tag = KOOPAS_GREEN;
				mario->GetTail()->ShowHitEffect();
			}
		}
	}
	// No collision occured, proceed normall
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else {
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		/*if (nx != 0) vx = nx * vx;*/

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			GetBoundingBox(mLeft, mTop, mRight, mBottom);
			e->obj->GetBoundingBox(oLeft, oTop, oRight, oBottom);
			if (dynamic_cast<CKoopas*>(e->obj)) {
				CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
				if (e->ny > 0)
					y = y;
				if (koopas->state == KOOPAS_STATE_SPINNING)
				{
					if (tag == KOOPAS_GREEN_PARA)
						tag = KOOPAS_GREEN;
					if (tag == KOOPAS_RED_PARA)
						tag = KOOPAS_RED;
					this->SetState(KOOPAS_STATE_DEATH);
					mario->AddScore(x, y, 100);
					koopas->x += koopas->dx;
				}
				else
				{
					if ((koopas->state == KOOPAS_STATE_SHELL_UP || koopas->state == KOOPAS_STATE_IN_SHELL)
						&& state == KOOPAS_STATE_WALKING)
					{
						x += dx;
						y += dy;
					}
					if (state == KOOPAS_STATE_WALKING && koopas->state == KOOPAS_STATE_WALKING)
					{
						x += dx;
						if (e->ny < 0)
							y += dy;
						this->vx = -this->vx;
						this->nx = -this->nx;
						koopas->vx = -koopas->vx;
						koopas->nx = -koopas->nx;
					}
				}
			}
			if (dynamic_cast<Block*>(e->obj))
			{
				if (e->ny < 0)
				{
					vy = 0;
					if (state == KOOPAS_STATE_SHELL_UP)
						vx = 0;
					if (tag == KOOPAS_RED && state == KOOPAS_STATE_WALKING)
					{
						if (this->nx > 0 && x >= e->obj->x + KOOPAS_TURN_DIFF)
							if (CalTurnable(e->obj, coObjects))
							{
								this->nx = -1;
								vx = this->nx * KOOPAS_WALKING_SPEED;
							}
						if (this->nx < 0 && x <= e->obj->x - KOOPAS_TURN_DIFF)
							if (CalTurnable(e->obj, coObjects))
							{
								this->nx = 1;
								vx = this->nx * KOOPAS_WALKING_SPEED;
							}
					}
					if (tag == KOOPAS_GREEN_PARA)
					{
						y = e->obj->y - KOOPAS_BBOX_HEIGHT;
						vy = -KOOPAS_JUMP_SPEED;
						vx = vx = this->nx * KOOPAS_WALKING_SPEED;
						this->nx = -1;
					}
				}
				else
				{
					if (e->nx != 0)
						x += dx;
					if (state == KOOPAS_STATE_SHELL_UP && e->ny > 0)
						y += dy;
				}
				//DebugOut(L"[KOOPAS] kx: %f ky: %f bx: %f by: %f\n",x,y, e->obj->x, e->obj->y);
			}
			if (dynamic_cast<CBrick*>(e->obj) || dynamic_cast<MusicBrick*>(e->obj)) {
				LPGAMEOBJECT object = NULL;
				if (dynamic_cast<CBrick*>(e->obj)) object = dynamic_cast<CBrick*>(e->obj);
				if (dynamic_cast<MusicBrick*>(e->obj)) object = dynamic_cast<MusicBrick*>(e->obj);
				object->GetBoundingBox(oLeft, oTop, oRight, oBottom);
				if (e->ny < 0) {
					this->vy = 0;
					if (state == KOOPAS_STATE_SHELL_UP)
						vx = 0;
					if (tag == KOOPAS_RED && state == KOOPAS_STATE_WALKING)
					{
						if (this->nx > 0 && x >= e->obj->x + KOOPAS_TURN_DIFF)
							if (CalTurnable(e->obj, coObjects))
							{
								this->nx = -1;
								vx = this->nx * KOOPAS_WALKING_SPEED;
							}
						if (this->nx < 0 && x <= e->obj->x - KOOPAS_TURN_DIFF)
							if (CalTurnable(e->obj, coObjects))
							{
								this->nx = 1;
								vx = this->nx * KOOPAS_WALKING_SPEED;
							}
					}
					if (tag == KOOPAS_GREEN_PARA || KOOPAS_GREEN) {
						this->nx = -1;
						vx = this->nx * KOOPAS_WALKING_SPEED;
					}
				}
				//if (e->ny != 0) vy = 0;
				if (e->nx != 0)
				{
					if (ceil(mBottom) != oTop)
					{
						vx = -vx;
						this->nx = -this->nx;
					}
				}
			}
			if (dynamic_cast<QuestionBrick*>(e->obj) && state == KOOPAS_STATE_SPINNING && e->nx != 0 && ceil(mBottom) != oTop) {
				QuestionBrick* qBrick = dynamic_cast<QuestionBrick*>(e->obj);
				if (qBrick->state != QUESTION_BRICK_HIT)
					qBrick->SetState(QUESTION_BRICK_HIT);
				if (e->nx != 0)
				{
					vx = -vx;
					this->nx = -this->nx;
				}
			}
			if (dynamic_cast<CGoomba*>(e->obj)) {
				x += dx;
				y -= dy;
			}
			if (dynamic_cast<BreakableBrick*>(e->obj) && state == KOOPAS_STATE_SPINNING && e->nx != 0 && ceil(mBottom) != oTop)
			{
				BreakableBrick* tmp = dynamic_cast<BreakableBrick*>(e->obj);
				tmp->Break();
				mario->AddScore(x, y, 150);
			}

			if (dynamic_cast<Coin*>(e->obj)) {
				x += dx;
				if (e->ny < 0) {
					y += dy;
				}
			}
		}
	}
}

bool CKoopas::CalTurnable(LPGAMEOBJECT object, vector<LPGAMEOBJECT>* coObjects)
{
	//if (!IsInViewPort())
	//	return false;
	for (UINT i = 0; i < coObjects->size(); i++)
		if (dynamic_cast<CBrick*>(coObjects->at(i)) || dynamic_cast<Block*>(coObjects->at(i)))
			if (abs(coObjects->at(i)->y == object->y))
			{
				if (nx > 0)
					if (coObjects->at(i)->x > object->x && coObjects->at(i)->x - 16 < object->x + 16)
						return false;
				if (nx < 0)
					if (coObjects->at(i)->x + 16 > object->x - 16 && coObjects->at(i)->x < object->x)
						return false;
			}
	return true;
}

void CKoopas::Render()
{
	int ani = -1;
	if (state == KOOPAS_STATE_SHELL_UP || state == KOOPAS_STATE_DEATH)
		ani = KOOPAS_ANI_SHELL_UP;
	else if (state == KOOPAS_STATE_IN_SHELL)
		ani = KOOPAS_ANI_SHELL;
	else if (state == KOOPAS_STATE_SPINNING)
	{
		if (vx < 0)
			ani = KOOPAS_ANI_SPIN_LEFT;
		else
			ani = KOOPAS_ANI_SPIN_RIGHT;
	}
	else
	{
		if (this->nx < 0)
			ani = KOOPAS_ANI_WALKING_LEFT;
		else
			ani = KOOPAS_ANI_WALKING_RIGHT;
	}
	if (state != KOOPAS_STATE_DEATH) {
		if (tag == KOOPAS_GREEN_PARA || tag == KOOPAS_RED_PARA)
			if (this->nx < 0)
				ani = KOOPAS_ANI_PARA_LEFT;
			else
				ani = KOOPAS_ANI_PARA_RIGHT;
		if (reviving_start != 0)
		{
			if (state == KOOPAS_STATE_IN_SHELL)
				ani = KOOPAS_ANI_SHAKE;
			if (state == KOOPAS_STATE_SHELL_UP)
				ani = KOOPAS_ANI_SHAKE_UP;
		}
	}
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_INACTIVE:
		vx = 0;
		vy = 0;
		break;
	case KOOPAS_STATE_DEATH:
		//y += KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT + 1;
		vx = 0;
		vy = 0;
		break;
	case KOOPAS_STATE_WALKING:
		vx = this->nx * KOOPAS_WALKING_SPEED;
		break;
	case KOOPAS_STATE_SPINNING:
		CMario* mario;
		mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		//if (nx > 0)
		vx = mario->nx * KOOPAS_WALKING_SPEED * 5;
		/*else
			vx = -KOOPAS_WALKING_SPEED * 5;*/
		break;
	case KOOPAS_STATE_IN_SHELL:
		vx = 0;
		vy = 0;
		StartShell();
		break;
	case KOOPAS_STATE_SHELL_UP:
		vy = -KOOPAS_SHELL_DEFLECT_SPEED;
		/*if (x <= mario->x)
			nx = -1;
		else*/
		nx = 1;
		vx = nx * KOOPAS_WALKING_SPEED;
		StartShell();
		break;
	}
}

void CKoopas::HandleBeingHeld(LPGAMEOBJECT player) {
	CMario* mario = dynamic_cast<CMario*>(player);
	if (isBeingHeld && mario->GetIsHolding()) {
		if (state == KOOPAS_STATE_IN_SHELL || state == KOOPAS_STATE_SHELL_UP) {
			if (mario->nx > 0) {
				x = mario->x + MARIO_BIG_BBOX_WIDTH * mario->nx - 3.0f;
			}
			else x = mario->x + MARIO_BIG_BBOX_WIDTH * mario->nx;
			if (mario->GetLevel() != MARIO_LEVEL_SMALL) {
				y = mario->y + 9.0f;
			}
			else {
				y = mario->y - 2.0f;
			}
			vy = 0;
		}
	}
	else if (isBeingHeld && !mario->GetIsHolding()) {
		if (state == KOOPAS_STATE_SHELL_UP || state == KOOPAS_STATE_IN_SHELL) {
			this->nx = mario->nx;
			isBeingHeld = false;
			mario->StartKicking();
			SetState(KOOPAS_STATE_SPINNING);
		}
	}
}

void CKoopas::CalRevivable() {
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	float cx = CGame::GetInstance()->GetCamPosX();
	if (mario == NULL) {
		return;
	}
	else {
		if (!IsInViewPort()) {
			isReviable = true;
		}
	}
}
