#pragma once
#include "Enemy.h"
#include "SlopeBrick.h"
// State
#define TIME_BOMB_STATE_IDLE		1
#define TIME_BOMB_STATE_FALL		2
#define TIME_BOMB_STATE_EXPLODE		3
#define TIME_BOMB_STATE_DIE			4
// ANI
#define TIME_BOMB_ANI_IDLE			0
#define TIME_BOMB_ANI_FALL			1
#define TIME_BOMB_ANI_EXPLODE			2

// BBox
#define TIME_BOMB_BBOX_WIDTH			14
#define TIME_BOMB_BBOX_HEIGHT		19
// Speed
#define BLACK_MONSTER_WALKING_SPEED		0.07f
// Time
#define TIME_BOMB_DIE_TIME			500
#define TIME_BOMB_THROW_TIME		2000
// Others
#define TIME_BOMB_DISTANCE_X		100

class CTimeBomb : public CEnemy
{
	float start_x = -1;

	DWORD die_start = 0;
	DWORD throw_time = 0;

	void SetBoundingBox();

public:
	CTimeBomb();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetState(int state);

	// Intersect with objs
	void OnIntersect(CGameObject* obj, vector<LPGAMEOBJECT>* coObjs);
};

