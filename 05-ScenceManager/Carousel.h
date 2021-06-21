#pragma once
#include "GameObject.h"

#define CAROUSEL_STATE_UP 1
#define CAROUSEL_STATE_BACK 2

#define CAROUSEL_SPEED 0.05f


class CCarousel : public CGameObject
{
private: 
	int mRight, mBottom;
	float carousel_speed;
	void SetBoundingBox();
public:
	CCarousel(float r, float b,int type);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {};
	//virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void SetState(int state);
	float GetBrickSpeed() { return carousel_speed; }
};