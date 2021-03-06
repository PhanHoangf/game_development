#pragma once
#include "GameObject.h"

#define GOOMBA_WALKING_SPEED		0.035f
#define GOOMBA_GRAVITY				0.002f
#define GOOMBA_DIE_DEFLECT_SPEED	0.35f
#define GOOMBA_JUMP_SPEED			0.125f
#define GOOMBA_HIGHJUMP_SPEED		0.25f


#define GOOMBA_RED_TIME_CHASING			7000
#define GOOMBA_RED_TIME_WALKING			500
#define GOOMBA_RED_TIME_HIGHJUMPING		500
#define GOOMBA_RED_TIME_JUMPING			125
#define GOOMBA_TIME_DIYING				250
#define GOOMBA_TIME_DIYING_BY_TAIL		1000

#define GOOMBA_BIG_TIME_WALKING		1000
#define GOOMBA_BIG_TIME_READY_FLY	1000
#define GOOMBA_BIG_TIME_DROP	1000

#define GOOMBA_RED_JUMPING_STACKS	3

#define GOOMBA_RED_RANGE_CHASING		50

#define GOOMBA_NORMAL_BBOX_WIDTH		16
#define GOOMBA_RED_BBOX_WIDTH			20
#define GOOMBA_NORMAL_BBOX_HEIGHT		15
#define GOOMBA_RED_BBOX_HEIGHT			17
#define GOOMBA_RED_BBOX_WINGS_HEIGHT	22

#define GOOMBA_BIG_BBOX_WIDTH			20
#define GOOMBA_BIG_BBOX_HEIGHT			19
#define GOOMBA_BIG_BBOX_WINGS_HEIGHT	24


#define GOOMBA_BBOX_HEIGHT_DIE			9

#define GOOMBA_STATE_WALKING			100
#define GOOMBA_STATE_DIE				200
#define GOOMBA_STATE_DIE_BY_TAIL		300
#define GOOMBA_STATE_RED_JUMPING		400
#define GOOMBA_STATE_RED_HIGHJUMPING	450
#define GOOMBA_STATE_RED_WINGSWALKING	500

//! BIG GOOMBA
#define GOOMBA_STATE_FLYING				550
#define GOOMBA_STATE_WINGSWALKING		600
#define GOOMBA_STATE_WINGSJUMPING		650

#define GOOMBA_NORMAL_ANI_WALKING		0
#define GOOMBA_NORMAL_ANI_DIE			1
#define GOOMBA_RED_ANI_JUMPING			2
#define GOOMBA_RED_ANI_WINGSWALKING		3
#define GOOMBA_RED_ANI_WALKING			4
#define GOOMBA_RED_ANI_DIE				5

//! BIG GOOMBA
#define GOOMBA_ANI_FLY	6
#define GOOMBA_ANI_WALK	7
#define GOOMBA_ANI_WING_WALK	8

#define GOOMBA_NORMAL			0
#define GOOMBA_RED				1
#define GOOMBA_RED_NORMAL		2
#define GOOMBA_BIG				3

#define MAX_POOP_GOOMBA	4

#define GOOMBA_ANISET_ID	2

class CGoomba : public CGameObject
{

	DWORD chasing_start = 0;
	DWORD walking_start = 0;
	DWORD start_flying = 0;
	DWORD start_drop = 0;
	DWORD start_ready_fly = 0;

	bool isDying = false;
	bool isWhackedDying = false;
	bool isWalking = false;
	bool isJumping = false;
	bool isHighJumping = false;
	bool isFlying = false;

	bool isDiedByKoopas = false;
	bool isDropGoomba = false;
	bool isReadyToFly = false;

	int poopGoomba = 0;

public:
	DWORD dying_start = 0;
	CGoomba();
	CGoomba(int tag);
	int jumping_stacks = 0;
	virtual void SetState(int state);
	
	void StartDying(bool l = false) { dying_start = GetTickCount64(); if (l) isWhackedDying = true; else isDying = true; }
	void StartChasing() { chasing_start = GetTickCount64(); }
	void StartWalking() { walking_start = GetTickCount64(); isWalking = true; }
	void StartDropGoomba() { start_drop = GetTickCount64(); isDropGoomba = true; }
	void StartFlying() { isFlying = true; start_drop = GetTickCount64(); }
	
	void StopDropGoomba() { start_drop = 0; isDropGoomba = false; }
	void StopWalking() { walking_start = 0; isWalking = false; }

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	//Get
	bool GetIsWalking() { return isWalking; }
	bool GetIsJumping() { return isJumping; }
	bool GetIsHighJumping() { return isHighJumping; }
	int GetJumpingStack() { return jumping_stacks; }
	bool GetIsWhackedDying() { return isWhackedDying; }
	bool GetIsDying() { return isDying; }
	DWORD GetChasingStart() { return chasing_start; }
	DWORD GetWalkingStart() { return walking_start; }
	DWORD GetDyingStart() { return dying_start; }

	//Set
	void SetIsWalking(bool isWalking) { this->isWalking = isWalking; }
	void SetIsJumping(bool isJumping) { this->isJumping = isJumping; }
	void SetIsHighJumping(bool isHighJumping) { this->isHighJumping = isHighJumping; }
	void SetJumpingStack(int jumping_stacks) { this->jumping_stacks = jumping_stacks; }
	void SetIsWhackedDying(bool isWhackedDying) { this->isWhackedDying = isWhackedDying; }
	void SetIsDying(bool isDying) { this->isDying = isDying; }
	void ResetStartWalking() { walking_start = 0; }
	void HandleBigGoomba();
	void SetSpeedDirection(float condition);
	void CreatePoopGoomba();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

};