#pragma once

#include "GameObject.h"
#define PORTAL_BBOX_WIDTH	28
#define PORTAL_BBOX_HEIGHT	1

/*
	Object that triggers scene switching
*/
class CPortal : public CGameObject
{
	int scene_id;	// target scene to switch to 

	int width;
	int height; 
public:
	CPortal(int scene_id, float x, float y);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	int GetSceneId() { return scene_id;  }
};