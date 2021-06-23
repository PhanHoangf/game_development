#include "MarioBullet.h"
#include "Brick.h"
#include "Mario.h"
#include "PlayScence.h"
#include "QuestionBrick.h"
#include "Block.h"
#include "PiranhaPlant.h"
#include "PiranhaPlantFire.h"

void MarioBullet::Render() {
	if (isDestroyed || !isAppear) return;
	int ani = -1;
	if (nx > 0) {
		ani = MARIO_BULLET_ANI_RIGHT;
	}
	else ani = MARIO_BULLET_ANI_LEFT;
	animation_set->at(ani)->Render(x, y);
}

void MarioBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (isDestroyed) return;
	
	if (!this->IsInViewPort()) {
		isDestroyed = true;
		mario->shootTimes--;
		DebugOut(L"IN");
	}

	int a = this->IsInViewPort() ? 1 : 0;
	DebugOut(L"vp::%d\n", a);

	

	CGameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	vy = ay * dt;

	if (state == MARIO_BULLET_STATE_UP) {
		if (y <= limitY) {
			SetState(MARIO_BULLET_STATE_DOWN);
		}
	}


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
		float oLeft, oTop, oRight, oBottom;
		float mLeft, mTop, mRight, mBottom;
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CBrick*>(e->obj) || dynamic_cast<QuestionBrick*>(e->obj) || dynamic_cast<Block*>(e->obj)) {

				x += min_tx * dx + nx * 0.4f;
				y += min_ty * dy + ny * 0.4f;

				if (e->ny < 0) {
					SetState(MARIO_BULLET_STATE_UP);
					SetLimitY(y);
				}
				if (e->nx != 0 || e->ny > 0) {
					x += dx;
					y += dy;
				}

				if (dynamic_cast<QuestionBrick*>(e->obj)) {
					if (e->nx != 0) {
						isDestroyed = true;
						mario->shootTimes--;
					}
				}
			}

			if (dynamic_cast<CGoomba*>(e->obj)) {
				e->obj->SetState(GOOMBA_STATE_DIE_BY_TAIL);
				isDestroyed = true;
				mario->AddScore(x, y, 100);
				mario->shootTimes--;
			}
			else if (dynamic_cast<CKoopas*>(e->obj)) {
				mario->AddScore(x, y, 100);
				e->obj->SetState(KOOPAS_STATE_DEATH);
				isDestroyed = true;
				mario->shootTimes--;
			}
			else if (dynamic_cast<PiranhaPlant*>(e->obj) || dynamic_cast<PiranhaPlantFire*>(e->obj)) {
				mario->AddScore(x, y, 100);
				e->obj->SetState(200);
				isDestroyed = true;
				mario->shootTimes--;
			}
			else
			{
				x += dx;
				y += dy;
			}
		}
	}

}

void MarioBullet::SetState(int state) {
	CGameObject::SetState(state);
	switch (state)
	{
	case MARIO_BULLET_STATE_DOWN:
		if (nx > 0)
			vx = MARIO_BULLET_SPEED_X;
		else
			vx = -MARIO_BULLET_SPEED_X;
		ay = MARIO_BULLET_SPEED_Y;
		break;
	case MARIO_BULLET_STATE_UP:
		if (nx > 0)
			vx = MARIO_BULLET_SPEED_X;
		else
			vx = -MARIO_BULLET_SPEED_X;
		ay = -MARIO_BULLET_SPEED_Y;
		break;
	default:
		break;
	}
}