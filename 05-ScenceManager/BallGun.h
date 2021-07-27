#pragma once
#include "Enemy.h"
// State
#define BALL_GUN_STATE_IDLE		1
#define BALL_GUN_STATE_ATTACK		2
#define BALL_GUN_STATE_DIE		3
// ANI
#define BALL_GUN_ANI_IDLE		0
#define BALL_GUN_ANI_ATTACK			1
// BBox
#define BALL_GUN_BBOX_WIDTH			16
#define BALL_GUN_BBOX_HEIGHT		16
// Speed
#define BLACK_MONSTER_WALKING_SPEED		0.07f
// Time
#define BALL_GUN_WAIT_TIME			3000
#define BALL_GUN_THROW_TIME		2000
// Others
#define BALL_GUN_DISTANCE_X		100

class CBallGun : public CEnemy
{
	float start_x = -1;

	DWORD wait_time = 0;
	DWORD throw_time = 0;

	void SetBoundingBox();

public:
	CBallGun();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetState(int state);

	// Intersect with objs
	void OnIntersect(CGameObject* obj, vector<LPGAMEOBJECT>* coObjs);
};

