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

	if (state == MUSIC_BRICK_STATE_DOWN) {
		if (isPushedDown) {
			if (y - start_y >= 8) {
				StopPushedDown();
				SetState(MUSIC_BRICK_STATE_UP);
			}
		}
		if (isGoDown) {
			if (y <= start_y) {
				y = start_y;
				SetState(MUSIC_BRICK_STATE_IDLE);
			}
		}
	}

	if (state == MUSIC_BRICK_STATE_UP) {
		if (isPushedUp) {
			if (start_y - y >= 8) {
				StopPushedUp();
				SetState(MUSIC_BRICK_STATE_DOWN);
			}
		}
		if (isGoUp) {
			if (y >= start_y) {
				y = start_y;
				SetState(MUSIC_BRICK_STATE_IDLE);
			}
		}
	}
}

void MusicBrick::SetState(int state) {
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
		vy = -MUSIC_BRICK_SPEED_UP;
		break;
	}
}