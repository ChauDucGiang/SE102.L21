#pragma once
#include "Enemy.h"
#include "Ground.h"
#include "Brick.h"
// State
#define WORM_STATE_WALKING			100
#define WORM_STATE_DIE				999
// Animation
#define WORM_ANI_WALKING			0
#define WORM_ANI_WALKING_RIGHT		1
#define WORM_ANI_SPIN_LEFT			2
#define WORM_ANI_SPIN_RIGHT			3
#define WORM_ANI_HIDE					4
#define WORM_ANI_SHAKE				5
#define WORM_ANI_WING					6
// BBox
#define WORM_BBOX_WIDTH				24
#define WORM_BBOX_HEIGHT				15
// Speed
#define WORM_GRAVITY					0.0007f
#define WORM_WING_GRAVITY				0.0005f
#define WORM_WALKING_SPEED			0.02f
#define WORM_WALKING_WING_SPEED		0.02f
#define WORM_SPIN_SPEED				0.215f
#define WORM_WING_JUMP				0.175f
#define WORM_WING_Y_SPEED				0.05f
#define WORM_DIE_X_SPEED				0.07f
#define WORM_DIE_Y_SPEED				0.2f
#define WORM_HIT_Y_SPEED				0.1f
// Type
#define WORM_GREEN					1
#define WORM_RED						2
#define WORM_GREEN_WING				3
#define WORM_RED_WING					4
#define WORM_HIDE						5
//Time
#define WORM_DIE_TIME				500


class CWorm : public CEnemy
{
	float start_x = -1;
	DWORD die_start = 0;
	DWORD wait_time = 0;
	DWORD throw_time = 0;

	void SetBoundingBox();
public:
	CWorm(float x);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
