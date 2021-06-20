#pragma once
#include "GameObject.h"

#define TAIL_BBOX_WIDTH		8
#define TAIL_BBOX_HEIGHT	6
#define TAIL_HIT_SPRITE_ID	10998	
#define TAIL_HIT_TIME 25

class Tail :
    public CGameObject
{
    DWORD hit_start = 0;
public:
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
    virtual void Render();
    Tail(float x, float y) { this->x = x; this->y = y; }
    void SetState(int state);
    void ShowHitEffect() { hit_start = GetTickCount64(); };
    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

