#pragma once
#include "GameObject.h"
#include "Brick.h"
#include "Define.h"
#include "Utils.h"

class CPlayer : public CGameObject {
public:
	int level = 0;
	
	bool isWalking = false;
	
	bool isChangeDirection = false;

	bool isJumping = false;
	bool isReadyToJump = true;

	bool isRunning = false;
	bool isReadyToRun = true;

	bool isOnGround = false;
public:

	CPlayer(float x, float y);
	CPlayer() {}

	virtual void RenderMarioSmall() = 0;
	virtual void SetState(int state);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	

	/*virtual void RenderMarioBig() = 0;
	virtual void RenderMarioFire() = 0;
	virtual void RenderMarioTanuki() = 0;

	virtual void UpdateMarioSmall(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) = 0;
	virtual void UpdateMarioBig(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) = 0;
	virtual void UpdateMarioFire(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) = 0;
	virtual void UpdateMarioTanuki(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) = 0;

	virtual void SetStateMarioSmall(int state) = 0;
	virtual void SetStateMarioBig(int state) = 0;
	virtual void SetStateMarioFire(int state) = 0;
	virtual void SetStateMarioTanuki(int state) = 0;*/

	void SetLevel(int level) { this->level = level; }
	
	virtual void LimitWalkingSpeed();
	virtual void LimitJump();
};