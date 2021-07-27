#pragma once
#include "GameObject.h"
#include "Water.h"
// State
#define BULLET_STATE_FIRE			100
#define BULLET_STATE_EXPLODE		200
#define BULLET_STATE_DISAPPEAR		999
// Animation
#define BULLET_ANI_FIRE				0
#define BULLET_ANI_EXPLODE			1
#define BULLET_ANI_SET				5100
// BBox
#define BULLET_BBOX_WIDTH			16
#define BULLET_BBOX_HEIGHT			16
// Speed
#define BULLET_GRAVITY				0.0003f
#define BULLET_X_SPEED				0.2f
#define BULLET_Y_SPEED				0.02f
#define BULLET_ENEMY_X_SPEED		0.02f
#define BULLET_ENEMY_NEAR_Y_SPEED	0.045f
#define BULLET_ENEMY_FAR_Y_SPEED	0.0175f
#define BULLET_BOUND_Y_SPEED		0.2f
// Time
#define BULLET_DIE_TIME				3000
#define BULLET_EXPLODE_TIME			300


class CBullet : public CGameObject
{
	void SetBoundingBox();
public:
	bool isEnemy = false;
	bool isOnGround = false;
	DWORD explode_start = 0;
	DWORD die_start = 0;

	CBullet();

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);

	// Giao nhau vs các obj
	void OnIntersect(CGameObject* obj, vector<LPGAMEOBJECT>* coObjs);
};

