#pragma once
#include "GameObject.h"
#include "Water.h"
// State
#define ELEVATOR_STATE_IDLE			1
#define ELEVATOR_STATE_FALL			2
// Animation
#define ELEVATOR_ANI_IDLE			0
#define ELEVATOR_ANI_FALL			1
// BBox
#define ELEVATOR_BBOX_WIDTH			36
#define ELEVATOR_BBOX_IDLE_HEIGHT	67
#define ELEVATOR_BBOX_FALL_HEIGHT	79
// Speed
#define ELEVATOR_GRAVITY				0.0003f
#define ELEVATOR_X_SPEED				0.02f
#define ELEVATOR_Y_SPEED				0.02f
// Distance Max
#define ELEVATOR_X_MAX					1900
// Time
#define ELEVATOR_DIE_TIME				3000
#define ELEVATOR_EXPLODE_TIME			300



class CElevator : public CGameObject
{
	void SetBoundingBox();
public:
	bool isEnemy = false;
	bool isOnGround = false;
	DWORD explode_start = 0;
	DWORD die_start = 0;

	CElevator();

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);

	// Giao nhau vs các obj
	void OnIntersect(CGameObject* obj, vector<LPGAMEOBJECT>* coObjs);
};

