#pragma once
#include "GameObject.h"

class CPlayer : public CGameObject {
public:
	int level = 0;

	CPlayer(int level) { this->level = level; }

	virtual void RenderMarioSmall() = 0;
	virtual void RenderMarioBig() = 0;
	virtual void RenderMarioFire() = 0;
	virtual void RenderMarioTanuki() = 0;

	virtual void UpdateMarioSmall(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) = 0;
	virtual void UpdateMarioBig(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) = 0;
	virtual void UpdateMarioFire(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) = 0;
	virtual void UpdateMarioTanuki(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) = 0;

	virtual void SetStateMarioSmall(int state) = 0;
	virtual void SetStateMarioBig(int state) = 0;
	virtual void SetStateMarioFire(int state) = 0;
	virtual void SetStateMarioTanuki(int state) = 0;
};