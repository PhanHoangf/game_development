#include "Point.h"

Point::Point(int score)
{
	vx = 0;
	vy = -SCORE_SPEED;
	isDestroyed = false;
	start_exist = GetTickCount64();
	state = score;
}

void Point::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	CGameObject::Update(dt);
	y += dy;
	if (GetTickCount64() - start_exist >= SCORE_TIME)
		isDestroyed = true;
}

void Point::Render() {
	if (state == 100)
		CSprites::GetInstance()->Get(SCORE_SPRITE_100_ID)->Draw(x, y);
	if (state == 200)
		CSprites::GetInstance()->Get(SCORE_SPRITE_200_ID)->Draw(x, y);
	if (state == 400)
		CSprites::GetInstance()->Get(SCORE_SPRITE_400_ID)->Draw(x, y);
	if (state == 800)
		CSprites::GetInstance()->Get(SCORE_SPRITE_800_ID)->Draw(x, y);
	if (state == 1000)
		CSprites::GetInstance()->Get(SCORE_SPRITE_1000_ID)->Draw(x, y);
	if (state == 2000)
		CSprites::GetInstance()->Get(SCORE_SPRITE_2000_ID)->Draw(x, y);
	if (state == 4000)
		CSprites::GetInstance()->Get(SCORE_SPRITE_4000_ID)->Draw(x, y);
	if (state == 8000)
		CSprites::GetInstance()->Get(SCORE_SPRITE_8000_ID)->Draw(x, y);
	if (state == 1)
		CSprites::GetInstance()->Get(SCORE_SPRITE_1UP_ID)->Draw(x, y);
}

void Point::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = r = b = t = 0;
}