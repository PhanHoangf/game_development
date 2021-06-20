#include "Goomba.h"
#include "Brick.h"
#include "Utils.h"
#include "QuestionBrick.h"
#include "Mario.h"
#include "PlayScence.h"
#include "Game.h"
#include "Koopas.h"
#include "Point.h"

CGoomba::CGoomba()
{
	SetState(GOOMBA_STATE_WALKING);
}

void CGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + GOOMBA_NORMAL_BBOX_WIDTH;

	if (state == GOOMBA_STATE_DIE)
		bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
	else
		bottom = y + GOOMBA_NORMAL_BBOX_HEIGHT;
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = currentScene->GetPlayer();

	if (GetTickCount64() - dying_start >= GOOMBA_TIME_DIYING && isDying)
	{
		isDying = false;
		isDestroyed = true;

		mario->AddScore(this->x, this->y, 100);
		/*if (dynamic_cast<CIntroScene*> (CGame::GetInstance()->GetCurrentScene()))
			mario->SetState(MARIO_STATE_WALKING_RIGHT);*/
		return;
	}

	if (GetTickCount64() - dying_start >= GOOMBA_TIME_DIYING_BY_TAIL && isWhackedDying)
	{
		isWhackedDying = false;
		isDestroyed = true;
		return;
	}

	vy += GOOMBA_GRAVITY * dt;

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
	if (mario != NULL && state != GOOMBA_STATE_DIE) {
		if (mario->isTuring && mario->GetLevel() == MARIO_LEVEL_TAIL && state != GOOMBA_STATE_DIE && state != GOOMBA_STATE_DIE_BY_TAIL)
		{
			mario->GetTail()->GetBoundingBox(mLeft, mTop, mRight, mBottom);
			GetBoundingBox(oLeft, oTop, oRight, oBottom);
			if (isColliding(floor(mLeft), mTop, ceil(mRight), mBottom))
			{
				mario->AddScore(x, y, 100);
				//SetDirection(mario->nx);
				SetState(GOOMBA_STATE_DIE_BY_TAIL);
				mario->GetTail()->ShowHitEffect();
				return;
			}
		}
	}
	if (coEvents.size() == 0 || state == GOOMBA_STATE_DIE_BY_TAIL) {
		x += dx;
		y += dy;
		//vy += GOOMBA_GRAVITY * dt;
	}
	else {
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		//x += min_tx * dx + nx * 0.4f;		// nx*PUSHBACK : need to push out a bit to avoid overlapping next frame
		//y += min_ty * dy + ny * 0.4f;

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
				if (ny != 0) {
					vy = 0;
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
			}
			if (dynamic_cast<CGoomba*>(e->obj)) {
				if (e->nx != 0) {
					x += dx;
					//y += dy;
				}
			}
			if (dynamic_cast<CKoopas*>(e->obj)) {
				if (e->nx != 0 && e->obj->GetState() == KOOPAS_STATE_SPINNING) {
					isDiedByKoopas = true;
					SetState(GOOMBA_STATE_DIE);
				}
			}
			/*if (dynamic_cast<CMario*>(e->obj)) {
				CPlayScene* current_scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
				CMario* mario = current_scene->GetPlayer();
				if (e->nx != 0) {
					mario->SetState(MARIO_STATE_DIE);
				}
			}*/
		}
	}


	if (vx < 0 && x < 0) {
		x = 0; vx = -vx;
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CGoomba::Render()
{
	int ani = GOOMBA_NORMAL_ANI_WALKING;
	if (state == GOOMBA_STATE_DIE) {
		if (isDiedByKoopas) ani = GOOMBA_NORMAL_ANI_DIE;
		else ani = GOOMBA_NORMAL_ANI_DIE;
	}

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	int previousState = state;
	CGameObject::SetState(state);
	switch (state)
	{
	case GOOMBA_STATE_DIE:
		y += GOOMBA_NORMAL_BBOX_WIDTH - GOOMBA_BBOX_HEIGHT_DIE - 1;
		vx = 0;
		vy = 0;
		if (isDiedByKoopas) vy = -MARIO_DIE_DEFLECT_SPEED;
		StartDying();
		break;
	case GOOMBA_STATE_WALKING:
		vx = -GOOMBA_WALKING_SPEED;
		break;
	case GOOMBA_STATE_DIE_BY_TAIL:
		vy = -GOOMBA_DIE_DEFLECT_SPEED;
		vx = -vx;
		ay = GOOMBA_GRAVITY;
		StartDying(true);
		break;
	}
}
