#pragma once
#include "GameObject.h"
// BBox
#define TREASURE_BBOX_WIDTH			10
#define TREASURE_BBOX_HEIGHT		14
#define TREASURE_ANI_SET			25
// Speed
#define TREASURE_GRAVITY			0.002f
#define TREASURE_Y_SPEED			0.4f

class CTreasure : public CGameObject
{
	void SetBoundingBox();
public:
	int type;
	float start_x = -1;			// Init position 
	float start_y = -1;
	//CTreasure();
	CTreasure(int type);
	CTreasure(float x, float y);

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

