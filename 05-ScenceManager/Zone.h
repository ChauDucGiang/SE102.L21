#pragma once
#pragma once
#include "GameObject.h"
// BBox
#define ZONE_BBOX_WIDTH			16
#define ZONE_BBOX_HEIGHT			16
#define ZONE_ANI_SET			25
// Speed

class CZone : public CGameObject
{
	void SetBoundingBox();
public:

	CZone();
	CZone(float x, float y);

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

