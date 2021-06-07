#pragma once
#include "GameObject.h"

#define STOP_POINT_WIDTH 16

class StopPoint :
	public CGameObject
{
	virtual void Render() {
		animation_set->at(0)->Render(x, y);

		RenderBoundingBox();
	};
	int moveLeft;
	int moveUp;
	int moveRight;
	int moveBottom;
	int sceneId;
public:

	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) {
		l = x;
		t = y;
		r = x + STOP_POINT_WIDTH;
		b = y + STOP_POINT_WIDTH;
	};

	void SetAcceptMovement(int mLeft, int mUp, int mRight, int mBottom) {
		this->moveLeft = mLeft;
		this->moveUp = mUp;
		this->moveRight = mRight;
		this->moveBottom = mBottom;
	}

	void GetAcceptMovement(int& mLeft, int& mUp, int& mRight, int& mBottom) {
		mLeft = this->moveLeft;
		mUp = this->moveUp;
		mRight = this->moveRight;
		mBottom = this->moveBottom;
	}
	void SetSceneId(int scnId) { sceneId = scnId; }
	void GetSceneId(int& scnId) { scnId = this->sceneId; }
};

