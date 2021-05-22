#include "MushRoom.h"
#include "GameObject.h"
#include "Mario.h"
#include "PlayScence.h"
#include "Block.h"
#include "QuestionBrick.h"

MushRoom::MushRoom() {
	SetAppear(false);
}

void MushRoom::Render() {
	if (isAppear) {
		animation_set->at(0)->Render(x, y);
		//RenderBoundingBox();
	}
}

void MushRoom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	CGameObject::Update(dt);
	float mLeft, mTop, mRight, mBottom;
	float oLeft, oTop, oRight, oBottom;
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = currentScene->GetPlayer();
	if (mario != NULL) {
		mario->GetBoundingBox(mLeft, mTop, mRight, mBottom);
		if (isColliding(mLeft, mTop, mRight, mBottom))
		{
			if (mario->GetLevel() == MARIO_LEVEL_SMALL) {
				mario->StartTransform(MARIO_LEVEL_BIG); 
				isAppear = false;
				isDestroyed = true;
			}

			/*if (tag == MUSHROOM_TYPE_RED)
			{

				mario->AddScore(x, y, 1000);
			}*/
			/*else
			{
				mario->AddScore(x, y, 1);
				mario->AddLife();
			}*/

		}
	}
	if (state == MUSHROOM_STATE_UP)
	{
		y += dy;
		if (start_y - y >= MUSHROOM_BBOX_HEIGHT)
		{
			vy = 0;
			y = start_y - MUSHROOM_BBOX_HEIGHT - 0.1f;
			SetState(MUSHROOM_STATE_RIGHT);
		}
	}
	if (state == MUSHROOM_STATE_RIGHT) {
		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		coEvents.clear();

		CalcPotentialCollisions(coObjects, coEvents);

		if (coEvents.size() == 0)
		{
			x += dx;
			y += dy;
			vy = MUSHROOM_GRAVITY;
		}
		else {
			float min_tx, min_ty, nx = 0, ny;
			float rdx = 0;
			float rdy = 0;

			// TODO: This is a very ugly designed function!!!!
			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

			// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
			//if (rdx != 0 && rdx!=dx)
			//	x += nx*abs(rdx); 
			// block every object first!
			x += min_tx * dx + nx * 0.4f;
			y += min_ty * dy + ny * 0.4f;

			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];
				GetBoundingBox(mLeft, mTop, mRight, mBottom);
				e->obj->GetBoundingBox(oLeft, oTop, oRight, oBottom);
				if (dynamic_cast<CBrick*>(e->obj) || dynamic_cast<QuestionBrick*>(e->obj))
				{
					if (e->ny < 0)
						vy = 0;
					if (e->nx != 0)
					{
						if (ceil(mBottom) != oTop)
							vx = -vx;
					}
				}
				else if (dynamic_cast<Block*>(e->obj))
				{
					Block* block = dynamic_cast<Block*>(e->obj);
					x += dx;
					if (ny < 0)
						vy = 0;
					else
						y += dy;
				}
				else
				{
					x += dx;
					y += dy;
				}
			}
			// clean up collision events
			for (UINT i = 0; i < coEvents.size(); i++)
				delete coEvents[i];
		}
	}
}

void MushRoom::SetState(int state) {
	CGameObject::SetState(state);
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (state)
	{
	case MUSHROOM_STATE_UP:
		vy = -0.05f;
		start_y = y;
		break;
	case MUSHROOM_STATE_RIGHT:
		vy = MUSHROOM_GRAVITY;
		vx = vx = mario->nx * MUSHROOM_SPEED;
		break;
	case MUSHROOM_STATE_LEFT:
		vy = MUSHROOM_GRAVITY;
		break;
	}
}