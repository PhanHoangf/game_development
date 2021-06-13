#include "MusicBrick.h"
#include "PlayScence.h"

MusicBrick::MusicBrick(float x, float y) {
	start_y = y;
	start_x = x;
	SetState(MUSIC_BRICK_STATE_IDLE);
}

void MusicBrick::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + MUSIC_BRICK_BBOX_WIDTH;
	bottom = y + MUSIC_BRICK_BBOX_WIDTH;
}

void MusicBrick::Render() {
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void MusicBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CGameObject::Update(dt);

	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();

	CMario* mario = currentScene->GetPlayer();

	y += dy;
	//DebugOut(L"y - start_y = %f\n", y - start_y);
	if (state == MUSIC_BRICK_STATE_DOWN) {
		if (isPushedDown) {
			if (y - start_y >= MUSIC_PUSH_MAX_HEIGHT) {
				StopPushedDown();
				SetState(MUSIC_BRICK_STATE_UP);
			}
		}
		if (isGoDown) {
			if (y >= start_y) {
				y = start_y;
				SetState(MUSIC_BRICK_STATE_IDLE);
			}
		}
	}

	if (state == MUSIC_BRICK_STATE_UP) {
		if (isPushedUp) {
			if (start_y - y >= MUSIC_PUSH_MAX_HEIGHT) {
				StopPushedUp();
				SetState(MUSIC_BRICK_STATE_DOWN);
			}
		}
		if (isGoUp) {
			if (y <= start_y) {
				y = start_y;
				SetState(MUSIC_BRICK_STATE_IDLE);
			}
		}
	}

	float oLeft, oTop, oRight, oBottom;
	float mLeft, mTop, mRight, mBottom;

	mario->GetBoundingBox(mLeft, mTop, mRight, mBottom);
	GetBoundingBox(oLeft, oTop, oRight, oBottom);

	if (isColliding(mLeft, mTop, mRight, mBottom)) {
		DebugOut(L"Mario->vy::%f\n", mario->vy);
		if (state == MUSIC_BRICK_STATE_UP) {
			mario->SetIsJumpOnMusicBrick(true);
			mario->vy = -0.6;
		}
	}
	else {
		mario->SetIsJumpOnMusicBrick(false);
	}

	//vector<LPCOLLISIONEVENT> coEvents;
	//vector<LPCOLLISIONEVENT> coEventsResult;

	//coEvents.clear();

	//CalcPotentialCollisions(coObjects, coEvents);

	//if (coEvents.size() == 0)
	//{
	//	x += dx;
	//	y += dy;
	//}
	//else
	//{
	//	float min_tx, min_ty, nx = 0, ny;
	//	float rdx = 0;
	//	float rdy = 0;

	//	// TODO: This is a very ugly designed function!!!!
	//	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

	//	//how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
	//	/*if (rdx != 0 && rdx != dx)
	//		x += nx * abs(rdx);*/

	//		// block every object first!
	//		/*x += min_tx * dx + nx * 0.4f;*/
	//	//y += min_ty * dy + ny * 0.4f;
	//	//if (nx != 0) vx = 0;
	//	//if (ny != 0) vy = 0;

	//	//
	//	// Collision logic with other objects
	//	//
	//	float oLeft, oTop, oRight, oBottom;
	//	float mLeft, mTop, mRight, mBottom;
	//	for (UINT i = 0; i < coEventsResult.size(); i++) {
	//		LPCOLLISIONEVENT e = coEventsResult[i];
	//		if (dynamic_cast<CMario*>(e->obj)) {
	//			if (e->ny > 0) {
	//				/*DebugOut(L"msBrick->y::%f\n", this->y);
	//				DebugOut(L"MARIO->y::%f\n", mario->y);*/
	//			}

	//		}
	//	}
	//}
}

void MusicBrick::SetState(int state) {
	int previousState = GetState();
	CGameObject::SetState(state);
	switch (state)
	{
	case MUSIC_BRICK_STATE_IDLE:
		vx = vy = 0;
		Reset();
		break;
	case MUSIC_BRICK_STATE_DOWN:
		vy = MUSIC_BRICK_SPEED;
		break;
	case MUSIC_BRICK_STATE_UP:
		vy = -MUSIC_BRICK_SPEED;
		break;
	}
}