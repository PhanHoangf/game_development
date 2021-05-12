#pragma once
#include "GameObject.h"

#define TYPE_MARIO 1

#define MARIO_WALKING_SPEED		0.15f 
//0.1f
#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_JUMP_DEFLECT_SPEED 0.2f
#define MARIO_GRAVITY			0.002f
#define MARIO_DIE_DEFLECT_SPEED	 0.5f



#define MARIO_ANI_BIG_IDLE_RIGHT		0
#define MARIO_ANI_BIG_IDLE_LEFT			1
#define MARIO_ANI_SMALL_IDLE_RIGHT		2
#define MARIO_ANI_SMALL_IDLE_LEFT			3

#define MARIO_ANI_BIG_WALKING_RIGHT			4
#define MARIO_ANI_BIG_WALKING_LEFT			5
#define MARIO_ANI_SMALL_WALKING_RIGHT		6
#define MARIO_ANI_SMALL_WALKING_LEFT		7

#define MARIO_ANI_DIE				8


#ifndef MARIO_LEVEL

#define	MARIO_LEVEL_SMALL	0
#define	MARIO_LEVEL_BIG		1
#define MARIO_LEVEL_TAIL	2
#define MARIO_LEVEL_FIRE	3

#endif // !MARIO_LEVEL

#ifndef MARIO_STATE

#define MARIO_STATE_IDLE	   -1
#define MARIO_STATE_WALKING		0
#define MARIO_STATE_MAX_SPEED	1
#define MARIO_STATE_JUMP		2
#define MARIO_STATE_BRAKE		3
#define MARIO_STATE_SITDOWN		4
#define MARIO_STATE_HOLD		5
#define MARIO_STATE_FLY			6
#define MARIO_STATE_KICK		7

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
#define MARIO_SMALL_BBOX_HEIGHT 15

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

#define MARIO_SMALL_IDLE_LEFT	7
#define MARIO_SMALL_WALK_LEFT	8
#define MARIO_SMALL_WALK_FAST_LEFT	9

#endif // !MARIO_SMALL_ANI_LEFT








#define MARIO_UNTOUCHABLE_TIME 5000

#define MARIO_WORLD_MAP_IDLE 0


class CMario : public CGameObject
{
	int level;
	int untouchable;
	DWORD untouchable_start;
	int direction;

	float start_x;			// initial position of Mario at scene
	float start_y;
public:
	CMario(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void SetDirection(int direct) { direction = direct; }
	void Reset();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};