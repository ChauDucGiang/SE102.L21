#include "TreasureTree.h"
#include "Game.h"

CTreasureTree::CTreasureTree()
{
	SetBoundingBox();
}

CTreasureTree::CTreasureTree(float x, float y)
{
	SetPosition(x, y);
	SetBoundingBox();
}

void CTreasureTree::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void CTreasureTree::Render()
{
	int ani = 0;
	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

void CTreasureTree::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + TREASURE_TREE_BBOX_WIDTH ;
	b = y - TREASURE_TREE_BBOX_HEIGHT;
	CGameObject::GetBoundingBox(l, t, r, b);
}

void CTreasureTree::SetBoundingBox()
{
	left = x;
	top = y;
	right = x + TREASURE_TREE_BBOX_WIDTH;
	bottom = y - TREASURE_TREE_BBOX_HEIGHT;
}
