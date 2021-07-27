#include "Treasure.h"
#include "Game.h"

CTreasure::CTreasure(int type)
{
	this->type = type;
	SetBoundingBox();
}

CTreasure::CTreasure(float x, float y)
{
	SetPosition(x, y);
	SetBoundingBox();
}

void CTreasure::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void CTreasure::Render()
{
	int ani = 0;
	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CTreasure::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + TREASURE_BBOX_WIDTH;
	b = y - TREASURE_BBOX_HEIGHT;
	CGameObject::GetBoundingBox(l, t, r, b);
}

void CTreasure::SetBoundingBox()
{
	left = x;
	top = y;
	right = x + TREASURE_BBOX_WIDTH;
	bottom = y - TREASURE_BBOX_HEIGHT;
}
