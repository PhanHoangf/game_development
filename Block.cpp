#include "Block.h"
void Block::Render() {
	//animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

void Block::GetBoundingBox(float& l, float& t, float& r, float& b) {
	l = x;
	t = y;
	r = x + 16;
	b = y + 10;
}