#pragma once
#include "GameObject.h"

#define PM_BBOX_WIDTH  32
#define PM_BBOX_HEIGHT 16

#define PM_STATE_WIDTH  1
#define PM_STATE_HEIGHT 2

#define PM_SPEED_MAX 0.1f
#define PM_SPEED 0.01f
#define PM_SPEED_ACCELERATION 0.0015f

class CPlatformsMoving : public CGameObject
{
public:
	int mRight, mBottom;
	int start;
	int end;
	bool col;
	void SetBoundingBox();
public:
	CPlatformsMoving(float r, float b,int type);
	void SetStart(int start) { this->start = start; }
	void SetEnd(int end) { this->end = end; }
	void Collision(LPGAMEOBJECT object, float& y);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetState(int state);
};