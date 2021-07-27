#pragma once
#pragma once
#include "GameObject.h"
// BBox
#define WATER_BBOX_WIDTH			14
#define WATER_BBOX_HEIGHT			12
#define WATER_ANI_SET			25
// Speed

class CWater : public CGameObject
{
	void SetBoundingBox();
public:

	CWater();
	CWater(float x, float y);

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

