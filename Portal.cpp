#include "Portal.h"


CPortal::CPortal(int scene_id, float x, float y)
{
	this->scene_id = scene_id;
	this->x = x;
	this->y = y;
}

void CPortal::Render()
{
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void CPortal::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + PORTAL_BBOX_WIDTH;
	b = y + PORTAL_BBOX_HEIGHT;
}