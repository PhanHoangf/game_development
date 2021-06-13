#include "BoomerangKoopas.h"
#include "Brick.h"
#include "Utils.h"
#include "PlayScence.h"

BoomerangKoopas::BoomerangKoopas(float x, float y) {
	this->nx = -1;
	this->start_x = x;
	SetState(BOOMERANG_KOOPAS_STATE_WALKING);
}

void BoomerangKoopas::Render() {
	int ani = BOOMERANG_KOOPAS_ANI_RIGHT;
	if (this->nx > 0) {
		int ani = BOOMERANG_KOOPAS_ANI_RIGHT;
	}
	if (this->nx < 0) {
		int ani = BOOMERANG_KOOPAS_ANI_LEFT;
	}
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void BoomerangKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {


	CGameObject::Update(dt, coObjects);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	vy += BOOMERANG_KOOPAS_GRAVITY * dt;
	DebugOut(L"vx::%f\n", vx);
	coEvents.clear();

	if (state != BOOMERANG_KOOPAS_STATE_DIE) {
		CalcPotentialCollisions(coObjects, coEvents);
	}

	if (GetTickCount64() - start_idle > IDLE_TIME) {
		DebugOut(L"IN\n");
		StopIdle();
		SetState(BOOMERANG_KOOPAS_STATE_WALKING);
	}
	else {
		SetState(BOOMERANG_KOOPAS_STATE_IDLE);
	}

	if (x <= start_x) {
		x = start_x;
		this->vx = BOOMERANG_KOOPAS_SPEED;
		if (!isIdle) {
			StartIdle();
		}
	}
	if (x + GetWidth() >= LIMIT_X) {
		x = LIMIT_X - BOOMERANG_KOOPAS_BBOX_WIDTH;
		this->vx = -BOOMERANG_KOOPAS_SPEED;
		if (!isIdle) {
			StartIdle();
		}
	}

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

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CBrick*>(e->obj)) {
				if (e->ny < 0) {
					vy = 0;
				}
				x += dx;
			}
		}

	}
}

void BoomerangKoopas::SetState(int state) {
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = currentScene->GetPlayer();
	CGameObject::SetState(state);
	switch (state)
	{
	case BOOMERANG_KOOPAS_STATE_IDLE:
		vx = 0;
		break;
	case BOOMERANG_KOOPAS_STATE_WALKING:

		if (x > mario->x)
			this->nx = -1;
		else
			this->nx = 1;

		vx = BOOMERANG_KOOPAS_SPEED;
		break;

	case BOOMERANG_KOOPAS_STATE_DIE:
		vy = -BOOMERANG_KOOPAS_DIE_REFLECT_SPEED;
		vx = 0;
		break;
	default:
		break;
	}
}