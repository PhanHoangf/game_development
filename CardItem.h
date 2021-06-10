#pragma once
#include "Utils.h"
#include "GameObject.h"

#define CARD_ANI_RANDOM		0
#define CARD_ANI_MUSHROOM	1
#define CARD_ANI_FIREFLOWER	2
#define CARD_ANI_STAR		3

#define CARD_STATE_RANDOM		0
#define CARD_STATE_MUSHROOM		1
#define CARD_STATE_FIREFLOWER	2
#define CARD_STATE_STAR			3

#define CARD_BBOX_WIDTH		16

#define CARD_SPEED 0.1f

#define CARD_TIME  1000
#define CARD_RANDOM_TIME 100

class CardItem : public CGameObject
{
public:
	CardItem();
	~CardItem();
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void SetState(int state) {};
	void SetAppear(bool ap) { this->isAppear = ap; }
private:
	DWORD start = 0;
	bool isAppear = true;
};
