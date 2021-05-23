#pragma once
#include "GameObject.h"

#define TYPE_MARIO 1

#define MARIO_WALKING_SPEED_START	0.001f 
#define MARIO_JUMP_SPEED_VX			0.0005f
//#define MARIO_WALKING_SPEED_MAX		0.15f
//#define MARIO_RUNNING_SPEED_MAX		0.15f
#define MARIO_SPEED_MAX				0.15f
#define MARIO_ACCELERATION			0.0003f
#define MARIO_WALKING_SPEED_MIN		0.05f

#define MARIO_WALKING_SPEED		0.15f
#define MARIO_ACCELERATION		0.0003f
//#define MARIO_JUMP_SPEED_Y		0.18f
#define MARIO_JUMP_DEFLECT_SPEED 0.2f
#define MARIO_GRAVITY			0.002f
#define MARIO_DIE_DEFLECT_SPEED	 0.5f

#define MARIO_ACCELERATION_JUMP		0.0005f
#define MARIO_JUMP_SPEED_MAX		0.28f
#define MARIO_JUMP_SPEED_MIN		0.18f
#define MARIO_SLOW_FALLING_SPEED	0.05f
#define MARIO_FLY_SPEED				0.135f

#define MARIO_JUMP_MAX	0.28f
#define MARIO_SUPER_JUMP_MAX 0.30f


#ifndef MARIO_LEVEL

#define	MARIO_LEVEL_SMALL	0
#define	MARIO_LEVEL_BIG		1
#define MARIO_LEVEL_TAIL	2
#define MARIO_LEVEL_FIRE	3

#endif // !MARIO_LEVEL

#ifndef MARIO_STATE

#define MARIO_STATE_WALKING_LEFT -2
#define MARIO_STATE_IDLE	   -1
#define MARIO_STATE_WALKING_RIGHT 0
#define MARIO_STATE_MAX_SPEED	1
#define MARIO_STATE_JUMP		2
#define MARIO_STATE_BRAKE		3
#define MARIO_STATE_SITDOWN		4
#define MARIO_STATE_HOLD		5
#define MARIO_STATE_FLY			6
#define MARIO_STATE_KICK		7
#define MARIO_STATE_JUMP_RIGHT	8
#define MARIO_STATE_JUMP_LEFT	9
#define MARIO_STATE_JUMP_X		10
#define MARIO_STATE_TRANSFORM	11

#endif // !MARIO_STATE

#ifndef MARIO_DIRECTION

#define MARIO_DIRECT_RIGHT 1
#define MARIO_DIRECT_LEFT -1
#define MARIO_DIRECT_IDLE  0

#endif // !MARIO_DIRECTION

#ifndef MARIO_BBOX

#define MARIO_BIG_BBOX_WIDTH  15
#define MARIO_BIG_BBOX_HEIGHT 27

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 16

#endif // !MARIO_BBOX

#ifndef MARIO_SMALL_ANI_RIGHT

#define MARIO_SMALL_RIGHT_IDLE	0
#define MARIO_SMALL_WALK_RIGHT	1
#define MARIO_SMALL_WALK_FAST_RIGHT	2
#define MARIO_MAX_SPEED_SMALL_RIGHT	3
#define MARIO_JUMP_SMALL_RIGHT	4
#define MARIO_JUMP_DOWN_SMALL_RIGHT	5
#define MARIO_BRAKE_SMALL_RIGHT	6

#endif // !MARIO_SMALL_ANI_RIGHT

#ifndef MARIO_SMALL_ANI_LEFT

#define MARIO_SMALL_LEFT_IDLE	7
#define MARIO_SMALL_WALK_LEFT	8
#define MARIO_SMALL_WALK_FAST_LEFT	9
#define MARIO_JUMP_SMALL_LEFT	11
#define MARIO_JUMP_DOWN_SMALL_LEFT	12
#define MARIO_BRAKE_SMALL_LEFT	13

#endif // !MARIO_SMALL_ANI_LEFT

#ifndef MARIO_BIG_ANI_RIGHT

#define MARIO_ANI_BIG_IDLE_RIGHT			14
#define MARIO_ANI_BIG_WALKING_RIGHT			15
#define MARIO_ANI_BIG_WALKING_FAST_RIGHT	16
#define MARIO_ANI_BIG_RUNNING_RIGHT			17
#define MARIO_ANI_BIG_JUMPINGUP_RIGHT		18
#define MARIO_ANI_BIG_JUMPINGDOWN_RIGHT		19
#define MARIO_ANI_BIG_BRAKING_RIGHT			20
#define MARIO_ANI_BIG_SITTING_RIGHT			21

#endif // !MARIO_BIG_ANI_RIGHT

#ifndef MARIO_BIG_ANI_LEFT

#define MARIO_ANI_BIG_IDLE_LEFT				22
#define MARIO_ANI_BIG_WALKING_LEFT			23
#define MARIO_ANI_BIG_WALKING_FAST_LEFT		24
#define MARIO_ANI_BIG_RUNNING_LEFT			25
#define MARIO_ANI_BIG_JUMPINGUP_LEFT		26
#define MARIO_ANI_BIG_JUMPINGDOWN_LEFT		27
#define MARIO_ANI_BIG_BRAKING_LEFT			28
#define MARIO_ANI_BIG_SITTING_LEFT			29

#endif // !MARIO_BIG_ANI_LEFT

#define MARIO_ANI_TRANSFORM_SMALL_RIGHT			115
#define MARIO_ANI_TRANSFORM_SMALL_LEFT			116

#define MARIO_UNTOUCHABLE_TIME 5000
#define MARIO_TRANSFORM_TIME 500

#define MARIO_WORLD_MAP_IDLE 0


class CMario : public CGameObject
{
	int level;
	int untouchable;
	DWORD untouchable_start;
	DWORD start_running;
	DWORD start_slowdown;
	DWORD start_transform;
	int direction;

	float start_x;			// initial position of Mario at scene
	float start_y;
	bool isOnGround = false;
	bool isRunning = false;
	bool isJumping = false;
	bool isSitting = false;
	bool isReadyToSit = true;
	bool isChangeDirection = false;
	bool isJumpingWithXButton = false;
	bool isTransforming = false;
	int runningStack;
public:
	CMario(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	int GetLevel() { return level; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void StartRunning() { start_running = GetTickCount(); }
	void SetDirection(int direct) { direction = direct; }
	bool GetIsOnGround() { return isOnGround; }
	void SetAccelerate(float accelerate);
	void Reset();
	void limitMarioSpeed(float& vx, int nx);
	void handleMarioJump();
	void HandleChangeDirection();
	void slowDownVx() { vx = int(abs(vx) / 2); }
	void pullDown() { ay = MARIO_GRAVITY; isJumping = false; isOnGround = true; }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void RenderMarioAniSmall(int& ani);
	void RenderMarioAniBig(int& ani);
	void StartTransform(int level) { isTransforming = true; start_transform = GetTickCount(); SetLevel(level); }
	void StopTransform() { isTransforming = false; start_transform = 0; }
	void handleSitDown();
	void HandleTransform(int level);
	void RenderMarioSit(int &ani);
};