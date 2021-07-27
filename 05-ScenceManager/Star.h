#pragma once
#include "GameObject.h"
#include "Water.h"
// State
#define STAR_STATE_FIRE			100
#define STAR_STATE_EXPLODE		200
#define STAR_STATE_DISAPPEAR		999
// Animation
#define STAR_ANI_FIRE				0
#define STAR_ANI_EXPLODE			1
#define STAR_ANI_SET				6050
// BBox
#define STAR_BBOX_WIDTH				22
#define STAR_BBOX_HEIGHT			26
// Speed
#define STAR_GRAVITY				0.0003f
#define STAR_X_SPEED				0.2f
#define STAR_Y_SPEED				0.02f
#define STAR_ENEMY_X_SPEED		0.02f
#define STAR_ENEMY_NEAR_Y_SPEED	0.045f
#define STAR_ENEMY_FAR_Y_SPEED	0.0175f
#define STAR_BOUND_Y_SPEED		0.2f
// Time
#define STAR_DIE_TIME				2000
#define STAR_EXPLODE_TIME			300


class CStar : public CGameObject
{
	void SetBoundingBox();
public:
	bool isEnemy = false;
	bool isOnGround = false;
	bool isFirstLand = false;
	float firstY = 0;
	DWORD explode_start = 0;
	DWORD die_start = 0;

	CStar();

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);

	// Giao nhau vs các obj
	void OnIntersect(CGameObject* obj, vector<LPGAMEOBJECT>* coObjs);
};

