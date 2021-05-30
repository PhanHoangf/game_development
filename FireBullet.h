#pragma once
#include "GameObject.h"
#define BULLET_BBOX_WIDTH	9
#define BULLET_BBOX_HEIGHT	9

#define BULLET_SPEED_X	0.05f
#define BULLET_SPEED_Y	0.05f


#define BULLET_ANI_RIGHT		0
#define BULLET_ANI_LEFT			1	

#define BULLET_ANI_SET_ID		9
class FireBullet :
    public CGameObject
{
    virtual void Render();
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
public:
    FireBullet(float bx, float by, bool up, bool right);
    virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

