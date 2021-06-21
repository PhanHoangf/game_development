#include "FireFlower.h"
#include "Mario.h"
#include "PlayScence.h"

FireFlower::FireFlower(float x, float y) {
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
}

void FireFlower::Render() {
	if (!isAppear || isDestroyed) return;
	animation_set->at(0)->Render(x, y);
}

void FireFlower::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (isDestroyed) return;

	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	CGameObject::Update(dt);

	x += dx;
	y += dy;


	if (state == FIRE_FLOWER_STATE_UP) {
		if (start_y - y >= FIRE_FLOWER_BBOX_HEIGHT) {
			y = start_y - FIRE_FLOWER_BBOX_HEIGHT;
			SetState(FIRE_FLOWER_STATE_IDLE);
		}
	}

}

void FireFlower::SetState(int state) {
	CGameObject::SetState(state);
	switch (state)
	{
	case FIRE_FLOWER_STATE_IDLE:
		vy = vx = 0;
		break;
	case FIRE_FLOWER_STATE_UP:
		vy = -FIRE_FLOWER_SPEED;
		break;
	default:
		break;
	}
}