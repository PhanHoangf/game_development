#pragma once
#include "GameObject.h"

class QuestionBrick :public CGameObject
{
	LPGAMEOBJECT item;
public:
	QuestionBrick();
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Setstate(int state);
	void SetItem(LPGAMEOBJECT item);

	LPGAMEOBJECT GetItem() {
		return item;
	}
	~QuestionBrick();
};

