#include "Water.h"
#include "Game.h"

CWater::CWater()
{
	SetBoundingBox();
}

CWater::CWater(float x, float y)
{
	SetPosition(x, y);
	SetBoundingBox();
}

void CWater::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void CWater::Render()
{
	int ani = 0;
	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

void CWater::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x + 2;
	t = y - 4;
	r = x + WATER_BBOX_WIDTH ;
	b = y - WATER_BBOX_HEIGHT;
	CGameObject::GetBoundingBox(l, t, r, b);
}

void CWater::SetBoundingBox()
{
	left = x;
	top = y - 4;
	right = x + WATER_BBOX_WIDTH;
	bottom = y - WATER_BBOX_HEIGHT;
}
