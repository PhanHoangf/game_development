#include "PipePortal.h"

CPipePortal::CPipePortal(int scene_id, float x, float y, int pipeUp)
{
	this->scene_id = scene_id;
	this->x = x;
	this->y = y;
	this->pipeUp = pipeUp;
}

void CPipePortal::Render()
{
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void CPipePortal::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + PORTAL_BBOX_WIDTH;
	b = y + PORTAL_BBOX_HEIGHT;
}