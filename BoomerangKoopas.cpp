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
	DebugOut(L"vx::%f\n", this->vx);


	CGameObject::Update(dt, coObjects);



	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	vy += BOOMERANG_KOOPAS_GRAVITY * dt;
	coEvents.clear();

	if (state != BOOMERANG_KOOPAS_STATE_DIE) {
		CalcPotentialCollisions(coObjects, coEvents);
	}


	if (x < start_x) {
		SetState(BOOMERANG_KOOPAS_STATE_WALKING);
	}


	if (x + GetWidth() >= LIMIT_X) {
		SetState(BOOMERANG_KOOPAS_STATE_WALKING);
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

		if (x <= start_x) {
			this->vx = BOOMERANG_KOOPAS_SPEED;
		}
		if (x + GetWidth() >= LIMIT_X) {
			this->vx = -BOOMERANG_KOOPAS_SPEED;
		}

		break;
	case BOOMERANG_KOOPAS_STATE_DIE:
		vy = -BOOMERANG_KOOPAS_DIE_REFLECT_SPEED;
		vx = 0;
		break;
	default:
		break;
	}
}

void BoomerangKoopas::ThrowBoomerang() {
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	this->boomerang = new Boomerang(this->x, this->y);
	LPANIMATION_SET ani_set = animation_sets->Get(BOOMERANG_ANI_SET_ID);
	this->boomerang->SetAnimationSet(ani_set);
	this->boomerang->SetPosition(this->x, this->y);
	this->boomerang->SetIsAppear(true);
	new Unit(currentScene->GetGrid(), this->boomerang, this->boomerang->x, this->boomerang->y, 0);
}