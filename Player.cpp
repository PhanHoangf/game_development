#include "Player.h"

void CPlayer::SetState(int state) {
	CGameObject::SetState(state);
	switch (state)
	{
	case MARIO_STATE_IDLE:
		if (vx > 0) {
			ax = -MARIO_ACCELERATION;
		}
		if (vx < 0) {
			ax = MARIO_ACCELERATION;
		}
		ay = MARIO_GRAVITY;
		if (abs(vx) <= MARIO_WALKING_SPEED_MIN)
		{
			vx = 0;
			ax = 0;
		}
		break;
	case MARIO_STATE_WALKING_RIGHT:
		ax = MARIO_ACCELERATION;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		ax = -MARIO_ACCELERATION;
		nx = -1;
		break;
	}
}