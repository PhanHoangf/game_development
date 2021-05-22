#include "Goomba.h"
#include "Brick.h"
#include "Utils.h"
#include "QuestionBrick.h"
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

	if (GetTickCount64() - dying_start >= GOOMBA_TIME_DIYING && isDying)
	{
		isDying = false;
		isDestroyed = true;
		/*if (dynamic_cast<CIntroScene*> (CGame::GetInstance()->GetCurrentScene()))
			mario->SetState(MARIO_STATE_WALKING_RIGHT);*/
		return;
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 
	//vy += GOOMBA_GRAVITY * dt;

	coEvents.clear();

	if (state != GOOMBA_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);


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

		x += min_tx * dx + nx * 0.4f;		// nx*PUSHBACK : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;
		//DebugOut(L"Goomba[nx]::%f\n", nx);

		for (UINT i = 0; i < coEventsResult.size(); i++) {
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (e->obj != NULL)
				if (e->obj->isDestroyed == true)
					continue;
			if (dynamic_cast<CBrick*>(e->obj) || dynamic_cast<QuestionBrick*>(e->obj)) {
				if (nx != 0) {
					vx = -vx;
				}
			}
			if (dynamic_cast<CGoomba*>(e->obj)) {
				if (e->nx != 0) {
					x += dx;
				}
			}
		}
	}


	if (vx < 0 && x < 0) {
		x = 0; vx = -vx;
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	/*if (vx > 0 && x > 290) {
		x = 290; vx = -vx;
	}*/
}

void CGoomba::Render()
{
	int ani = GOOMBA_NORMAL_ANI_WALKING;
	if (state == GOOMBA_STATE_DIE) {
		ani = GOOMBA_NORMAL_ANI_DIE;
	}

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GOOMBA_STATE_DIE:
		y += GOOMBA_NORMAL_BBOX_WIDTH - GOOMBA_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		StartDying();
		break;
	case GOOMBA_STATE_WALKING:
		vx = -GOOMBA_WALKING_SPEED;
	}
}
