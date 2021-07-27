#pragma once
#include "GameObject.h"
// BBox
#define TREASURE_TREE_BBOX_WIDTH			22
#define TREASURE_TREE_BBOX_HEIGHT			44
#define TREASURE_TREE_ANI_SET			25
// Speed
#define TREASURE_TREE_GRAVITY			0.002f
#define TREASURE_TREE_Y_SPEED			0.4f

class CTreasureTree : public CGameObject
{
	void SetBoundingBox();
public:
	float start_x = -1;			// Init position of coin
	float start_y = -1;
	CTreasureTree();
	CTreasureTree(float x, float y);

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

