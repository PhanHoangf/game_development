#include "QuestionBrick.h"

QuestionBrick::QuestionBrick() {

}

void QuestionBrick::Render() {


	RenderBoundingBox();
}


void QuestionBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

}

void QuestionBrick::SetItem(LPGAMEOBJECT item) {
	this->item = item;
}

void QuestionBrick::Setstate(int state) {
	CGameObject::SetState(state);
	switch (state)
	{
	default:
		break;
	}
}