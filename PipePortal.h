#pragma once
#include "GameObject.h"

#define PORTAL_BBOX_WIDTH	28
#define PORTAL_BBOX_HEIGHT	1
#define TO_MAIN_SCENE	2
#define TO_EXTRA_SCENE	1

#define PIPE_UP	1
#define PIPE_DOWN 0

class CPipePortal : public CGameObject
{
	int scene_id;	// target scene to switch to 

	int width;
	int height;
	

public:
	float extra_start_x;
	float extra_start_y;
	int pipeUp;

	CPipePortal(int scene_id, float x, float y, int pipeUp, float start_x, float start_y, int tag);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	int GetSceneId() { return scene_id; }
};