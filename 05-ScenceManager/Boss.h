#pragma once
#include "Enemy.h"
#include "Ground.h"
#include "BlackMonster.h"
// State
#define BOSS_STATE_IDLE				1
#define BOSS_STATE_ATTACK			2
#define BOSS_STATE_WALKING			3
#define BOSS_STATE_DIE				4
// Animation
#define BOSS_ANI_IDLE				0
#define BOSS_ANI_ATTACK				1
#define BOSS_ANI_WALKING			2
#define BOSS_ANI_DIE				3
// BBox
#define BOSS_BBOX_WIDTH				33
#define BOSS_BBOX_HEIGHT			46	
// Speed
#define BOSS_GRAVITY				0.0007f
#define BOSS_WALKING_SPEED			0.02f
#define BOSS_SPIN_SPEED				0.215f
#define BOSS_DIE_X_SPEED			0.07f
#define BOSS_DIE_Y_SPEED			0.2f
#define BOSS_HIT_Y_SPEED			0.1f
// TIME 
#define BOSS_TIME_ATTACK			4000
#define BOSS_DIE_TIME				500

class CBoss : public CEnemy
{
	float start_x = -1;
	DWORD die_start = 0;
	DWORD wait_time = 0;
	DWORD throw_time = 0;
	CBlackMonster* monster = NULL;
	void SetBoundingBox();
public:
	int hp_boss = 2;
	CBoss(float x);
	CBlackMonster* listEnemy[5];
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
