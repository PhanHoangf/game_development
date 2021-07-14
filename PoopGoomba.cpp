#include "PoopGoomba.h"
#include "Mario.h"
#include "PlayScence.h"

void CPoopGoomba::Render() {
	animation_set->at(POOP_GOOMBA_ANI)->Render(x, y);
}

void CPoopGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	CGameObject::Update(dt);
	x += dx;
	y += dy;

	vy = ay * dt;

	float mLeft, mTop, mRight, mBottom;
	float oLeft, oTop, oRight, oBottom;

	if (mario != NULL) {
		mario->GetBoundingBox(mLeft, mTop, mRight, mBottom);
		if (isColliding(mLeft, mTop, mRight, mBottom)) {
			isCollide = true;
		}

		if (isCollide) {
			this->x = mario->x;
			DebugOut(L"[ma->y]::%f\n", mario->y);
			DebugOut(L"[ma->bt]::%f\n", mBottom);
			vx = 0.002f;
		}
	}
}