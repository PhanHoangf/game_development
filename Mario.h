#pragma once
#include "GameObject.h"
#include "Tail.h"

#define TYPE_MARIO 1

#define MARIO_WALKING_SPEED_START	0.001f 
#define MARIO_JUMP_SPEED_VX			0.0005f
#define MARIO_SPEED_MAX				0.15f
#define MARIO_ACCELERATION			0.0003f
#define MARIO_WALKING_SPEED_MIN		0.05f
#define MARIO_RUNNING_SPEED_MAX		0.2f
#define MARIO_SPEED_STACK	7
#define MARIO_SPEED_STACK_TIME	150
#define MARIO_SPEED_STACK_LOST_TIME	200

#define MARIO_WALKING_SPEED		0.15f
#define MARIO_ACCELERATION		0.0003f
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
#define MARIO_FLYING_MAX 0.35f

#ifndef MARIO_LEVEL

#define	MARIO_LEVEL_SMALL	0
#define	MARIO_LEVEL_BIG		1
#define MARIO_LEVEL_TAIL	2
#define MARIO_LEVEL_FIRE	3

#endif // !MARIO_LEVEL

#ifndef MARIO_STATE

#define MARIO_STATE_WALKING_LEFT		-2
#define MARIO_STATE_IDLE				-1
#define MARIO_STATE_WALKING_RIGHT		0
#define MARIO_STATE_MAX_SPEED			1
#define MARIO_STATE_JUMP				2
#define MARIO_STATE_BRAKE				3
#define MARIO_STATE_SITDOWN				4
#define MARIO_STATE_HOLD				5
#define MARIO_STATE_FLY					6
#define MARIO_STATE_KICK				7
#define MARIO_STATE_JUMP_RIGHT			8
#define MARIO_STATE_JUMP_LEFT			9
#define MARIO_STATE_JUMP_X				10
#define MARIO_STATE_TRANSFORM			11
#define MARIO_STATE_DIE					12
#define MARIO_STATE_READY_TO_HOLD		13
#define MARIO_STATE_TAIL_ATTACK			14

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

#define MARIO_BBOX_SIT_HEIGHT	18

#endif // !MARIO_BBOX

#ifndef MARIO_SMALL_ANI_RIGHT

#define MARIO_SMALL_RIGHT_IDLE					0
#define MARIO_SMALL_WALK_RIGHT					1
#define MARIO_SMALL_WALK_FAST_RIGHT				2
#define MARIO_MAX_SPEED_SMALL_RIGHT				3
#define MARIO_JUMP_SMALL_RIGHT					4
#define MARIO_JUMP_DOWN_SMALL_RIGHT				5
#define MARIO_BRAKE_SMALL_RIGHT					6
#define MARIO_ANI_SMALL_HOLD_IDLE_RIGHT			63
#define MARIO_ANI_SMALL_HOLD_WALKING_RIGHT		64
#define MARIO_ANI_SMALL_HOLD_JUMPING_RIGHT		65
#define MARIO_ANI_SMALL_KICKING_RIGHT			66
#define MARIO_ANI_SMALL_FLY_RIGHT				103

#endif // !MARIO_SMALL_ANI_RIGHT

#ifndef MARIO_SMALL_ANI_LEFT

#define MARIO_SMALL_LEFT_IDLE					7
#define MARIO_SMALL_WALK_LEFT					8
#define MARIO_SMALL_WALK_FAST_LEFT				9
#define MARIO_MAX_SPEED_SMALL_LEFT				10
#define MARIO_JUMP_SMALL_LEFT					11
#define MARIO_JUMP_DOWN_SMALL_LEFT				12
#define MARIO_BRAKE_SMALL_LEFT					13
#define MARIO_ANI_SMALL_HOLD_IDLE_LEFT			67
#define MARIO_ANI_SMALL_HOLD_WALKING_LEFT		68
#define MARIO_ANI_SMALL_HOLD_JUMPING_LEFT		69
#define MARIO_ANI_SMALL_KICKING_LEFT			70
#define MARIO_ANI_SMALL_FLY_LEFT				104

#endif // !MARIO_SMALL_ANI_LEFT

#ifndef MARIO_BIG_ANI_RIGHT

#define MARIO_ANI_BIG_IDLE_RIGHT				14
#define MARIO_ANI_BIG_WALKING_RIGHT				15
#define MARIO_ANI_BIG_WALKING_FAST_RIGHT		16
#define MARIO_ANI_BIG_RUNNING_RIGHT				17
#define MARIO_ANI_BIG_JUMPINGUP_RIGHT			18
#define MARIO_ANI_BIG_JUMPINGDOWN_RIGHT			19
#define MARIO_ANI_BIG_BRAKING_RIGHT				20
#define MARIO_ANI_BIG_SITTING_RIGHT				21
#define MARIO_ANI_BIG_KICKING_RIGHT				74

#define MARIO_ANI_BIG_HOLD_IDLE_RIGHT			71
#define MARIO_ANI_BIG_HOLD_WALKING_RIGHT		72
#define MARIO_ANI_BIG_HOLD_JUMPING_RIGHT		73

#define MARIO_ANI_BIG_FLY_LEFT					106

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
#define MARIO_ANI_BIG_KICKING_LEFT			78
#define MARIO_ANI_BIG_FLY_RIGHT				105

#define MARIO_ANI_BIG_HOLD_IDLE_LEFT		75
#define MARIO_ANI_BIG_HOLD_WALKING_LEFT		76
#define MARIO_ANI_BIG_HOLD_JUMPING_LEFT		77

#endif // !MARIO_BIG_ANI_LEFT

#ifndef MARIO_TAIL_ANI_LEFT

#define MARIO_ANI_TAIL_IDLE_LEFT			38
#define MARIO_ANI_TAIL_WALKING_LEFT			39
#define MARIO_ANI_TAIL_WALKING_FAST_LEFT	40
#define MARIO_ANI_TAIL_RUNNING_LEFT			41
#define MARIO_ANI_TAIL_JUMPINGUP_LEFT		42
#define MARIO_ANI_TAIL_JUMPINGDOWN_LEFT		43
#define MARIO_ANI_TAIL_BRAKING_LEFT			44
#define MARIO_ANI_TAIL_SITTING_LEFT			45

#define MARIO_ANI_TAIL_HOLD_IDLE_LEFT			83
#define MARIO_ANI_TAIL_HOLD_WALKING_LEFT		84
#define MARIO_ANI_TAIL_HOLD_WALKING_FAST_LEFT	84
#define MARIO_ANI_TAIL_HOLD_RUNNING_LEFT		84
#define MARIO_ANI_TAIL_HOLD_JUMPINGUP_LEFT		85
#define MARIO_ANI_TAIL_HOLD_JUMPINGDOWN_LEFT	85
#define MARIO_ANI_TAIL_HOLD_BRAKING_LEFT		84
#define MARIO_ANI_TAIL_KICKING_LEFT				86

#define MARIO_ANI_TAIL_TURNING_LEFT				96
#define MARIO_ANI_TAIL_FLAPPING_LEFT			98

#define MARIO_ANI_TAIL_FLY_UP_LEFT				110

#define MARIO_SPRITE_WHACK_LEFT_1_ID	12813
#define MARIO_SPRITE_WHACK_LEFT_2_ID	12814
#define MARIO_SPRITE_WHACK_LEFT_3_ID	12815
#define MARIO_SPRITE_WHACK_LEFT_4_ID	12816


#endif // !MARIO_TAIL_ANI_LEFT


#ifndef MARIO_TAIL_ANI_RIGHT

#define MARIO_ANI_TAIL_IDLE_RIGHT			30
#define MARIO_ANI_TAIL_WALKING_RIGHT		31
#define MARIO_ANI_TAIL_WALKING_FAST_RIGHT	32
#define MARIO_ANI_TAIL_RUNNING_RIGHT		33
#define MARIO_ANI_TAIL_JUMPINGUP_RIGHT		34
#define MARIO_ANI_TAIL_JUMPINGDOWN_RIGHT	35
#define MARIO_ANI_TAIL_BRAKING_RIGHT		36
#define MARIO_ANI_TAIL_SITTING_RIGHT		37

#define MARIO_ANI_TAIL_HOLD_IDLE_RIGHT			79
#define MARIO_ANI_TAIL_HOLD_WALKING_RIGHT		80
#define MARIO_ANI_TAIL_HOLD_WALKING_FAST_RIGHT	80
#define MARIO_ANI_TAIL_HOLD_RUNNING_RIGHT		80
#define MARIO_ANI_TAIL_HOLD_JUMPINGUP_RIGHT		81
#define MARIO_ANI_TAIL_HOLD_JUMPINGDOWN_RIGHT	81
#define MARIO_ANI_TAIL_HOLD_BRAKING_RIGHT		80
#define MARIO_ANI_TAIL_KICKING_RIGHT			82

#define MARIO_ANI_TAIL_TURNING_RIGHT			95
#define MARIO_ANI_TAIL_FLAPPING_RIGHT			97

#define MARIO_ANI_TAIL_FLY_UP_RIGHT				107

#define MARIO_SPRITE_WHACK_RIGHT_1_ID	12803
#define MARIO_SPRITE_WHACK_RIGHT_2_ID	12804
#define MARIO_SPRITE_WHACK_RIGHT_3_ID	12805
#define MARIO_SPRITE_WHACK_RIGHT_4_ID	12806

#endif // !MARIO_TAIL_ANI_RIGHT

#ifndef MARIO_FIRE_RIGHT

#define MARIO_ANI_FIRE_IDLE_RIGHT			46
#define MARIO_ANI_FIRE_WALKING_RIGHT		47
#define MARIO_ANI_FIRE_WALKING_FAST_RIGHT	48
#define MARIO_ANI_FIRE_RUNNING_RIGHT		49
#define MARIO_ANI_FIRE_JUMPINGUP_RIGHT		50
#define MARIO_ANI_FIRE_JUMPINGDOWN_RIGHT	51
#define MARIO_ANI_FIRE_BRAKING_RIGHT		52
#define MARIO_ANI_FIRE_SITTING_RIGHT		53

#define MARIO_ANI_FIRE_HOLD_IDLE_RIGHT			87
#define MARIO_ANI_FIRE_HOLD_WALKING_RIGHT		88
#define MARIO_ANI_FIRE_HOLD_WALKING_FAST_RIGHT	88
#define MARIO_ANI_FIRE_HOLD_RUNNING_RIGHT		88
#define MARIO_ANI_FIRE_HOLD_JUMPINGUP_RIGHT		89
#define MARIO_ANI_FIRE_HOLD_JUMPINGDOWN_RIGHT	89
#define MARIO_ANI_FIRE_HOLD_BRAKING_RIGHT		88
#define MARIO_ANI_FIRE_KICKING_RIGHT			90

#define MARIO_ANI_SHOOTING_JUMP_RIGHT			101
#define MARIO_ANI_SHOOTING_RIGHT				99

#define MARIO_ANI_FIRE_FLY_RIGHT				113

#endif // !MARIO_FIRE_RIGHT

#ifndef MARIO_FIRE_LEFT

#define MARIO_ANI_FIRE_IDLE_LEFT			54
#define MARIO_ANI_FIRE_WALKING_LEFT			55
#define MARIO_ANI_FIRE_WALKING_FAST_LEFT	56
#define MARIO_ANI_FIRE_RUNNING_LEFT			57
#define MARIO_ANI_FIRE_JUMPINGUP_LEFT		58
#define MARIO_ANI_FIRE_JUMPINGDOWN_LEFT		59
#define MARIO_ANI_FIRE_BRAKING_LEFT			60
#define MARIO_ANI_FIRE_SITTING_LEFT			61

#define MARIO_ANI_FIRE_HOLD_IDLE_LEFT			91
#define MARIO_ANI_FIRE_HOLD_WALKING_LEFT		92
#define MARIO_ANI_FIRE_HOLD_WALKING_FAST_LEFT	92
#define MARIO_ANI_FIRE_HOLD_RUNNING_LEFT		92
#define MARIO_ANI_FIRE_HOLD_JUMPINGUP_LEFT		93
#define MARIO_ANI_FIRE_HOLD_JUMPINGDOWN_LEFT	93
#define MARIO_ANI_FIRE_HOLD_BRAKING_LEFT		92
#define MARIO_ANI_FIRE_KICKING_LEFT				94

#define MARIO_ANI_SHOOTING_LEFT					100
#define MARIO_ANI_SHOOTING_JUMP_LEFT			102

#define MARIO_ANI_FIRE_FLY_LEFT					114

#endif // !MARIO_FIRE_LEFT





#define MARIO_ANI_TRANSFORM_SMALL_RIGHT			115
#define MARIO_ANI_TRANSFORM_SMALL_LEFT			116
#define MARIO_ANI_TRANSFORM_BANG				117
#define MARIO_ANI_DIE	62

#define MARIO_UNTOUCHABLE_TIME 5000
#define MARIO_TRANSFORM_TIME 500
#define MARIO_KICKING_TIME	200	
#define MARIO_FLYING_TIME	5000
#define MARIO_SHOOTING_TIME			150

#define MARIO_WORLD_MAP_IDLE 0

#define MARIO_DIE_DEFLECT_SPEED		0.5f

#define MARIO_TURNING_TOTAL_TIME	350
#define MARIO_TURING_TIME			70
#define MARIO_TURING_STACK  5
#define MARIO_NORMAL_FLY_MAX 0.35f

#define MARIO_MAX_BULLET	2


class CMario : public CGameObject
{
	int level;
	int untouchable;
	DWORD untouchable_start;
	DWORD start_running;
	DWORD start_slowdown;
	DWORD start_transform;
	DWORD start_kicking;
	DWORD start_turning;
	DWORD start_turning_state;
	DWORD start_speed_stack;
	DWORD running_stop;
	DWORD fly_start;
	DWORD tail_fly_start;
	DWORD start_shooting;
	int direction;

	bool isOnGround = false;

	
	bool isSitting = false;
	bool isReadyToSit = true;
	bool isChangeDirection = false;
	bool isJumpingWithXButton = false;
	bool isTransforming = false;
	bool isChangingY = false;
	bool isChagingX = false;
	bool isReadyToHold = false;
	bool isHolding = false;
	int runningStack;
	float tempY;
	bool isJumpOnMusicBrick = false;

	bool isRunning = false;
	bool isReadyToRun = false;

	bool isReadyToFly = false;
	bool isAttacked = false;
	bool isBangAni = false;
	Tail* tail;


public:
	bool isTuring = false;
	int turningStack = 0;
	bool isKicking = false;
	int marioScore = 0;
	int coin = 0;
	bool isFlapping = false;
	int speedStack = 0;

	bool normalFlyPullDown = false;
	bool isFlying = false;

	bool tailFlyPullDown = false;
	bool isTailFlying = false;

	bool isShooting = false;

	float x0, y0;

	int shootTimes = 0;
	bool isJumping = false;

	vector<int> cards;

	CMario(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	void Reset();
	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void SetAccelerate(float accelerate);
	void SetDirection(int direct) { nx = direct; }
	void SetHolding(bool hold) { isHolding = hold; }
	void SetIsReadyToHold(bool hold) { isReadyToHold = hold; }
	void SetIsJumpOnMusicBrick(bool isJump) { isJumpOnMusicBrick = isJump; }
	void SetIsReadyToRun(bool run) { isReadyToRun = run; }

	bool GetIsOnGround() { return isOnGround; }
	int GetLevel() { return level; }
	bool GetIsHolding() { return isHolding; }
	bool GetIsReadyToHold() { return isReadyToHold; }
	int GetUntouchable() { return untouchable; }
	bool GetIsReadyToRun() { return isReadyToRun; }
	Tail* GetTail() { return this->tail; }

	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }
	void StartTransform(int level) { if (this->level == MARIO_LEVEL_TAIL) isBangAni = true; isTransforming = true; start_transform = GetTickCount64(); SetLevel(level); }
	void StartKicking() { start_kicking = GetTickCount64(); isKicking = true; }
	void StartRunning() { start_running = GetTickCount64(); }
	void StartTurning() { start_turning_state = GetTickCount64(); isTuring = true; }
	void StartSpeedStack() { start_speed_stack = GetTickCount64(); }
	void StartFlying() { fly_start = GetTickCount64(); }
	void StartTailFlying() { tail_fly_start = GetTickCount64(); }
	void StartShooting() { start_shooting = GetTickCount64(); isShooting = true; }

	void StopTransform() { isTransforming = false; start_transform = 0; isChangingY = false; }
	void StopKicking() { start_kicking = 0; isKicking = false; }
	void StopSpeedStack() { start_speed_stack = 0; }
	void StopShooting() { start_shooting = 0; isShooting = false; }

	void limitMarioSpeed(float& vx, int nx);
	void slowDownVx() { vx = int(abs(vx) / 2); }

	void RenderMarioAniSmall(int& ani);
	void RenderMarioAniBig(int& ani);
	void RenderMarioAniTail(int& ani);
	void RenderMarioAniFire(int& ani);

	void handleMarioJump();
	void handleSitDown();
	void HandleTransform(int level);
	void RenderMarioSit(int& ani);
	void HandleMarioKicking();
	void HandleChangeDirection();
	void HandleChangeYTransform();
	void HandleMarioHolding();
	void HandleBasicMarioDie();
	void HandleChangeXTail();
	void HandleTurning();
	void HandleFlapping();
	void HandleSpeedStack();
	void HandleFlying();
	void HandleTailFlying();
	void HandleShooting();


	void pullDown() {
		if (!isFlapping) ay = MARIO_GRAVITY; isJumping = false; isOnGround = true;
	}
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void AddScore(float x, float y, int score);
	void AddCoin() { this->coin++; }
	void AddCard(int cardId) { cards.push_back(cardId); }

	//void InitTail();

	void ShootFireBall();
};