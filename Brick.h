#pragma once
#include "GameObject.h"

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

#define BRICK_SPEED 0.05f

#define PUSH_UP 1
#define PUSH_TO_RIGHT 2

#define ITEM_COIN 1
#define ITEM_MUSHROOM 2
#define ITEM_LEAF 3

#define BRICK_STATE_HIT 1
#define BRICK_STATE_IDLE 0
#define BRICK_STATE_LEFT 2
#define BRICK_STATE_DOWN 3

#define PUSH_MAX 8

class CBrick : public CGameObject
{
	int type = 0;
	int item = 0;
	int totalItems = 0;

	bool isPushed = false;
public:
	CBrick(float x = 0, float y = 0, int type = 0, int item = 0, int totalItems = 0); //! type means can push -- 1: PUSH UP, 2: PUSH TO RIGHT, item means item in brick -- 1: COIN, 2: MUSHROOM, 3: LEAF
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	
	void SetState(int state);
	void Pushed(int startPoint);

	void StartPushed() { isPushed = true; }
	void StopPushed() { isPushed = false; }

	void CreateItem(int typeItem);
	LPGAMEOBJECT SetUpItem(int typeItem);
};