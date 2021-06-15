#include "boomerang.h"

Boomerang::Boomerang(float x, float y) {
	this->start_x = x;
	this->start_y = y;
	SetLimitBoomerangY(y);
}

void Boomerang::Render() {
	if (!isAppear || isDestroyed) return;
	int ani = -1;
	if (state == BOOMERANG_STATE_IDLE) {
		ani = BOOMERANG_ANI_IDLE_LEFT;
	}
	if (state == BOOMERANG_STATE_THROW) {
		ani = BOOMERANG_ANI_THROW_LEFT;
	}
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void Boomerang::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CGameObject::Update(dt);

	/*DebugOut(L"[Y]::%f\n", y);
	DebugOut(L"[limitY]::%f\n", limitY);*/

	if (y <= limitY) {
		vy = BOOMERANG_GRAVITY;
		ax = -this->nx * BOOMERANG_ACCELERATION;
	}

	vx += ax * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

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

		//how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
	/*	if (rdx != 0 && rdx != dx)
			x += nx * abs(rdx);*/
			// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;
		//if (nx != 0) vx = 0;
		//if (ny != 0) vy = 0;

		//
		// Collision logic with other objects
		//
		float oLeft, oTop, oRight, oBottom;
		float mLeft, mTop, mRight, mBottom;
		for (UINT i = 0; i < coEventsResult.size(); i++) {
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CBrick*>(e->obj)) {
				if (e->ny != 0) {
					vy = 0;
					//vx = 0.15f;
				}
			}
			if (dynamic_cast<Block*>(e->obj)) {
				if (e->nx != 0 || e->ny != 0) {
					x += dx;
					y += dy;
				}
			}
		}
	}

}

void Boomerang::SetState(int state) {
	CGameObject::SetState(state);

	switch (state)
	{
	case BOOMERANG_STATE_IDLE:
		vx = vy = 0;
		break;
	case BOOMERANG_STATE_THROW:
		ax = this->nx * BOOMERANG_ACCELERATION;
		vy = -0.04f;
		break;
	default:
		break;
	}
}