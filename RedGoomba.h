#pragma once
#include "Goomba.h"

class RedGoomba :
    public CGoomba
{
    virtual void Render();
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
public:
    void SetState(int state);
    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

