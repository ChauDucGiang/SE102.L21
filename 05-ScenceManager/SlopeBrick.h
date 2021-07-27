#pragma once
#include "GameObject.h"
#include <math.h> 
#include "Bullet.h"
#include "TimeBomb.h"
#define SLOPE_STATE_UP 2
#define SLOPE_STATE_DOWN 1
// Speed
#define BRICK_SPEED 0.05f
// Other
#define SLOPE_DEFAULT_RATIO	0.49


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
	void SetRatio(float ratio) {		
		this->ratio_hw = ratio;
	}
};

