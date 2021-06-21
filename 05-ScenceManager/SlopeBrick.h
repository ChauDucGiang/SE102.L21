#pragma once
#include "GameObject.h"
#include <math.h> 

#define SLOPE_STATE_UP 2
#define SLOPE_STATE_DOWN 1

#define BRICK_SPEED 0.05f


class CSlopeBrick : public CGameObject
{
private:
	float mRight;
	float mBottom;
	
	double length;
	void SetBoundingBox();
public:
	float ratio_hw;
	CSlopeBrick(float r, float b, int type);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Collision(LPGAMEOBJECT object, float dy, float dx);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {};
	void SetState(int state);

};

#pragma once
