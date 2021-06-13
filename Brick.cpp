#include "Brick.h"
#include "PlayScence.h"
#include "Leaf.h"
#include "Coin.h"

CBrick::CBrick(float x, float y, int type, int item, int totalItems) :CGameObject() {
	this->start_x = x;
	this->start_y = y;
	this->type = type;
	this->item = item;
	if (totalItems == 0 && item != 0) {
		this->totalItems = 1;
	}
	this->totalItems = totalItems;
}

void CBrick::Render()
{
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

void CBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CGameObject::Update(dt);

	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = currentScene->GetPlayer();

	CGameObject::Update(dt);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	if (coEvents.size() == 0)
	{
		y += dy;
		x += dx;
	}
	if (state == BRICK_STATE_HIT && isPushed) {
		if (this->type == PUSH_TO_RIGHT) {
			if (x - start_x >= PUSH_MAX) {
				StopPushed();
				SetState(BRICK_STATE_LEFT);
			}
		}
		if (this->type == PUSH_UP) {
			if (start_y - y >= PUSH_MAX) {
				CreateItem(this->item);
				StopPushed();
				SetState(BRICK_STATE_DOWN);
			}
		}
	}
	if (state == BRICK_STATE_LEFT) {
		if (x <= start_x) {
			x = start_x;
			SetState(BRICK_STATE_IDLE);
 			CreateItem(this->item);
		}
	}
	if (state == BRICK_STATE_DOWN) {
		if (y >= start_y) {
			y = start_y;
			SetState(BRICK_STATE_IDLE);
		}
	}
	/*float mLeft, mTop, mRight, mBottom;
	float oLeft, oTop, oRight, oBottom;

	mario->GetBoundingBox(mLeft, mTop, mRight, mBottom);
	GetBoundingBox(oLeft, oTop, oRight, oBottom);*/
}

void CBrick::CreateItem(int typeItem) {
	if (this->totalItems == 0) return;
	this->totalItems--;
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CGameObject* object = SetUpItem(typeItem);

	if (dynamic_cast<Leaf*>(object)) {
		Leaf* lObject = dynamic_cast<Leaf*>(object);
		lObject->SetAppear(true);
		lObject->SetPosition(x, y);
		lObject->SetState(LEAF_STATE_UP);
		Unit* unit = new Unit(currentScene->GetGrid(), lObject, lObject->x, lObject->y, 0);
	}

	if (dynamic_cast<Coin*>(object)) {
		Coin* cObject = dynamic_cast<Coin*>(object);
		cObject->SetAppear(true);
		cObject->SetPosition(x, y);
		cObject->SetState(COIN_STATE_UP);
		Unit* unit = new Unit(currentScene->GetGrid(), cObject, cObject->x, cObject->y, 0);
	}

}

LPGAMEOBJECT CBrick::SetUpItem(int typeItem) {
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = currentScene->GetPlayer();
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPGAMEOBJECT object = NULL;

	if (typeItem == ITEM_LEAF) {
		object = new Leaf();
		LPANIMATION_SET ani_set = animation_sets->Get(LEAF_ANI_SET_ID);
		object->SetAnimationSet(ani_set);
	}

	if (typeItem == ITEM_COIN) {
		object = new Coin();
		LPANIMATION_SET ani_set = animation_sets->Get(COIN_ANI_SET_ID);
		object->SetAnimationSet(ani_set);
	}

	return object;

}

void CBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}

void CBrick::SetState(int state) {
	CGameObject::SetState(state);

	switch (state)
	{
	case BRICK_STATE_HIT:
		if (this->type == PUSH_UP) {
			vy = -BRICK_SPEED;
		}
		if (this->type == PUSH_TO_RIGHT) {
			vx = BRICK_SPEED;
		}
		StartPushed();
		break;
	case BRICK_STATE_IDLE:
		vx = vy = 0;
		break;
	case BRICK_STATE_LEFT:
		vx = -BRICK_SPEED;
		break;
	case BRICK_STATE_DOWN:
		vy = BRICK_SPEED;
		break;
	}
}