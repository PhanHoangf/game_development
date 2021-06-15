#pragma once
#include "Player.h"
#include "Brick.h"

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 16

class CMarioSmall :
    public CPlayer
{
public:
    CMarioSmall(float x, float y);
   
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
    virtual void Render();
    virtual void RenderMarioSmall();
    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
   
    void SetState(int state);
};

