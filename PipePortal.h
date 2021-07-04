#pragma once
#include "GameObject.h"

#define PORTAL_BBOX_WIDTH	28
#define PORTAL_BBOX_HEIGHT	1

class CPipePortal : public CGameObject
{
	int scene_id;	// target scene to switch to 

	int width;
	int height;
	int pipeUp;
public:
	CPipePortal(int scene_id, float x, float y, int pipeUp);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	int GetSceneId() { return scene_id; }
};