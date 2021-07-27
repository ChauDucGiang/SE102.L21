#include "YumetaroDieEffect.h"

void CYumetaroDieEffect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	// Calculate dx, dy 
	CGameObject::Update(dt);

	x += dx;
	y += dy;
}

void CYumetaroDieEffect::Render()
{
	int ani = YUMETARODIEEFFECT_ANI_ACTIVE;
	int alpha = 255;
	animation_set->at(ani)->Render(x, y);
}

void CYumetaroDieEffect::SetState(int state) {}

void CYumetaroDieEffect::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + YUMETARODIEEFFECT_BBOX_WIDTH;
	bottom = top - YUMETARODIEEFFECT_BBOX_HEIGHT;
}