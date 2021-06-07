#include "GreenBush.h"

void GreenBush::Render() {
	animation_set->at(0)->Render(x, y);

	RenderBoundingBox();
}