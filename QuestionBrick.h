#pragma once
#include "GameObject.h"

#define QUESTIONBRICK_TYPE_MARK		0

#define QUESTION_BRICK_NORMAL	0
#define QUESTION_BRICK_HIT	1

#define QUESTION_BRICK_ANI_NORMAL	0
#define QUESTION_BRICK_ANI_HIT	1

#define QUESTIONBRICK_SPEED		0.05f
#define QUESTIONBRICK_PUSH_MAX_HEIGHT 8

#define ITEM_COIN	0

class QuestionBrick :public CGameObject
{
	LPGAMEOBJECT item;
	bool isBeingPushedUp = false;
	bool isFallingDown = false;
public:
	QuestionBrick(int tag = ITEM_COIN, int type = QUESTIONBRICK_TYPE_MARK);
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetState(int state);
	void SetItem(LPGAMEOBJECT item);
	void startPushedUp();
	void stopPushedUp();
	LPGAMEOBJECT GetItem() {
		return item;
	}
	~QuestionBrick();
};

