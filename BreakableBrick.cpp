#include "BreakableBrick.h"
#include "GameObject.h"
#include "PlayScence.h"
#include "BreakPiece.h"
#include "Coin.h"

void BreakableBrick::Render()
{
	if (isDestroyed)
		return;
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void BreakableBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CGameObject::Update(dt);
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	float mLeft, mTop, mRight, mBottom;
	if (mario != NULL) {
		mario->GetTail()->GetBoundingBox(mLeft, mTop, mRight, mBottom);
		if (isColliding(mLeft, mTop, mRight, mBottom) && mario->isTuring) {
			this->Break();
		}
	}

	if (isDestroyed)
		return;

}

void BreakableBrick::Break() {
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(PIECE_ANI_SET_ID);

	BreakPiece* bPieceTopLeft = new BreakPiece(-1, 1);
	bPieceTopLeft->SetPosition(x, y);
	bPieceTopLeft->SetAnimationSet(ani_set);

	BreakPiece* bPieceBottomLeft = new BreakPiece(-1, -1);
	bPieceBottomLeft->SetPosition(x, y);
	bPieceBottomLeft->SetAnimationSet(ani_set);

	BreakPiece* bPieceTopRight = new BreakPiece(1, 1);
	bPieceTopRight->SetPosition(x, y);
	bPieceTopRight->SetAnimationSet(ani_set);

	BreakPiece* bPieceBottomRight = new BreakPiece(1, -1);
	bPieceBottomRight->SetPosition(x, y);
	bPieceBottomRight->SetAnimationSet(ani_set);

	currentScene->AddSpecialObject(bPieceTopLeft);
	currentScene->AddSpecialObject(bPieceBottomLeft);
	currentScene->AddSpecialObject(bPieceTopRight);
	currentScene->AddSpecialObject(bPieceBottomRight);

	isDestroyed = true;
}

void BreakableBrick::ChangeToCoin() {
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	vector<LPGAMEOBJECT> objects = currentScene->GetObjects();
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(COIN_ANI_SET_ID);
	for (size_t i = 0; i < objects.size(); i++)
	{
		if (dynamic_cast<BreakableBrick*>(objects.at(i)) && !objects.at(i)->isDestroyed) {
			BreakableBrick* bBrick = dynamic_cast<BreakableBrick*>(objects.at(i));
			Coin* coin = new Coin();
			coin->SetPosition(bBrick->x, bBrick->y);
			coin->SetAppear(true);
			coin->SetAnimationSet(ani_set);
			currentScene->AddSpecialObject(coin);
			bBrick->isDestroyed = true;
		}
	}
}