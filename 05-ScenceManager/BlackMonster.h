#pragma once
#include "Enemy.h"
// State
#define BLACKMONSTER_STATE_WALKING		1
#define BLACKMONSTER_STATE_JUMP			2
#define BLACKMONSTER_STATE_DIE_REVERSE	3
#define BLACKMONSTER_STATE_DIE			4
// State
#define BLACKMONSTER_ANI_WALKING		0
#define BLACKMONSTER_ANI_JUMP			1
// BBox
#define BLACKMONSTER_BBOX_WIDTH			16
#define BLACKMONSTER_BBOX_HEIGHT		16
// Speed
#define BLACK_MONSTER_WALKING_SPEED		0.07f
// Time
#define BLACKMONSTER_WAIT_TIME			3500
#define BLACKMONSTER_THROW_TIME		2000
#define BLACKMONSTER_DIE_TIME		500

//ANI_Set
#define BLACKMONSTER_ANI_SET		60
// Others
#define BLACKMONSTER_DISTANCE_X		100
#define BLACK_MONSTER_DISTANCE_MIN_X 100
#define BLACK_MONSTER_DISTANCE_MAX_X 120

class CBlackMonster : public CEnemy
{
	float start_x = -1;

	DWORD wait_time = 0;
	DWORD throw_time = 0;
	DWORD die_start = 0;
	void SetBoundingBox();
	
public:
	bool isOnWindow = false;
	bool isOnGround = false;
	CBlackMonster(float x);
	CBlackMonster(float x, float y);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetState(int state);

	
	// Intersect with objs
	void OnIntersect(CGameObject* obj, vector<LPGAMEOBJECT>* coObjs);
};

