#include "Zone.h"
#include "Game.h"

CZone::CZone()
{
	SetBoundingBox();
}

CZone::CZone(float x, float y)
{
	SetPosition(x, y);
	SetBoundingBox();
}

void CZone::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void CZone::Render()
{
	int ani = 0;
	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();

}

void CZone::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + ZONE_BBOX_WIDTH;
	b = y + ZONE_BBOX_HEIGHT;
	CGameObject::GetBoundingBox(l, t, r, b);
}

void CZone::SetBoundingBox()
{
	left = x;
	top = y;
	right = x + ZONE_BBOX_WIDTH;
	bottom = y + ZONE_BBOX_HEIGHT;
}
