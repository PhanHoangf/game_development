#include "BoomerangKoopas.h"
#include "Brick.h"
#include "Utils.h"
#include "PlayScence.h"

BoomerangKoopas::BoomerangKoopas(float x, float y) {
	this->nx = -1;
	this->start_x = x;
	SetState(BOOMERANG_KOOPAS_STATE_WALKING);
	this->SetThrowingPoint();
}

void BoomerangKoopas::Render() {
	int ani = BOOMERANG_KOOPAS_ANI_RIGHT;

	if (this->nx > 0) {
		if (state == BOOMERANG_KOOPAS_STATE_THROW) {
			ani = BOOMERANG_KOOPAS_ANI_THROW_RIGHT;
		}
		else ani = BOOMERANG_KOOPAS_ANI_RIGHT;
	}
	if (this->nx < 0) {
		if (state == BOOMERANG_KOOPAS_STATE_THROW) {
			ani = BOOMERANG_KOOPAS_ANI_THROW_LEFT;
		}
		else ani = BOOMERANG_KOOPAS_ANI_LEFT;
	}

	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void BoomerangKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	//DebugOut(L"vx::%f\n", this->vx);

	CGameObject::Update(dt, coObjects);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;


	vy += BOOMERANG_KOOPAS_GRAVITY * dt;
	coEvents.clear();

	if (state != BOOMERANG_KOOPAS_STATE_DIE) {
		CalcPotentialCollisions(coObjects, coEvents);

		if (GetTickCount64() - start_idle >= IDLE_TIME) {
			StopIdle();
		}

		if (x >= start_x && !isIdle) {
			SetState(BOOMERANG_KOOPAS_STATE_WALKING);
		}

		if (x < start_x) {
			x = start_x;
			StartIdle();
			vx = 0;
			this->points.clear();

		}

		if (x + GetWidth() > LIMIT_X) {
			x = LIMIT_X - GetWidth();
			StartIdle();
			vx = 0;
			this->points.clear();
			ThrowBoomerang();
		}

		if (x == LIMIT_X - GetWidth() && !isIdle) {
			SetState(BOOMERANG_KOOPAS_STATE_WALKING);
		}

		if (IsGoPassThrowPoint(x)) {
			//DebugOut(L"IN\n");
			state = BOOMERANG_KOOPAS_STATE_THROW;
			if (!isHoldingBoomerang && currentBoomerang < 1) {
				HoldBoomerang();
				isHoldingBoomerang = true;
			}
		}

		if (this->boomerang != NULL) {
			if (!this->boomerang->IsInViewPort())
				this->boomerang->isDestroyed = true;
		}

		if (isHoldingBoomerang) this->boomerang->SetPosition(this->x + 8, this->y - 4);
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
			if (dynamic_cast<Boomerang*>(e->obj)) {
				Boomerang* bRang = dynamic_cast<Boomerang*>(e->obj);
				if (e->nx != 0) {
					bRang->isDestroyed = true;
				}
			}
		}

	}
}

void BoomerangKoopas::SetState(int state) {
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = currentScene->GetPlayer();
	CGameObject::SetState(state);
	DebugOut(L"[bKoopas->state]::%d", state);
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
			if (isHoldingBoomerang) {
				ThrowBoomerang();
			}
		}
		if (x == LIMIT_X - GetWidth()) {
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
	this->boomerang->SetState(BOOMERANG_STATE_THROW);
	isHoldingBoomerang = false;
}

bool BoomerangKoopas::IsGoPassThrowPoint(float x) {
	this->points.push_back(x);
	for (size_t i = 0; i < this->points.size(); i++) {
		if (vx > 0 && this->points.at(i) > this->throwingPoint) {
			return true;
		}
		if (vx < 0 && this->points.at(i) < this->throwingPoint) {
			return true;
		}
	}
	return false;
}

void BoomerangKoopas::HoldBoomerang() {
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	if (boomerangs.size() < 2) {
		Boomerang* boomerang = new Boomerang(this->x, this->y);
		LPANIMATION_SET ani_set = animation_sets->Get(BOOMERANG_ANI_SET_ID);
		boomerang->SetAnimationSet(ani_set);
		if (nx < 0) {
			boomerang->SetPosition(this->x + 8, this->y - 4);
			boomerang->nx = -1;
		}
		if (nx > 0) {
			boomerang->SetPosition(this->x - 8, this->y - 4);
			boomerang->nx = 1;
		}
		this->boomerang->SetIsAppear(true);
		this->boomerang->SetState(BOOMERANG_STATE_IDLE);
		new Unit(currentScene->GetGrid(), boomerang, boomerang->x, boomerang->y, 0);
		boomerangs.push_back(boomerang);
	}
	//currentScene->AddSpecialObject(this->boomerang);
}