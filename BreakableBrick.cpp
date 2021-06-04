#include "BreakableBrick.h"
#include "GameObject.h"
#include "PlayScence.h"
#include "BreakPiece.h"

void BreakableBrick::Render()
{
	if (isDestroyed)
		return;
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void BreakableBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CGameObject::Update(dt);

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

	currentScene->AddObject(bPieceTopLeft);
	currentScene->AddObject(bPieceBottomLeft);
	currentScene->AddObject(bPieceTopRight);
	currentScene->AddObject(bPieceBottomRight);

	isDestroyed = true;
}