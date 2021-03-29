#pragma once
#include "GameObject.h"
#include "Bullet.h"
// State
#define YUMETARO_STATE_IDLE					0
#define YUMETARO_STATE_WALK					1
#define YUMETARO_STATE_JUMP					2
#define YUMETARO_STATE_DIE						999
// Animation
#define YUMETARO_ANI_IDLE						0
#define YUMETARO_ANI_WALK						1
#define YUMETARO_ANI_JUMP						2
// BBox
#define YUMETARO_BBOX_WIDTH				16
#define YUMETARO_BBOX_HEIGHT				19
// Time
#define YUMETARO_UNTOUCHABLE_TIME				3500
#define YUMETARO_DIE_TIME						3000
#define YUMETARO_KICK_TIME						100
#define YUMETARO_TAIL_TIME						210
#define YUMETARO_FIRE_BULLET_TIME				150
#define YUMETARO_WAG_FLY_TIME					300
#define YUMETARO_FLY_LIMIT_TIME				5000
#define YUMETARO_PIPE_DOWN_TIME				1000
#define YUMETARO_PIPE_UP_TIME					1500
#define YUMETARO_PIPE_UP_TIME_2				2500
#define YUMETARO_PIPE_UP_TIME_3				1000
#define YUMETARO_EAT_ITEM_TIME					1
#define YUMETARO_POWER_INCREASE_TIME			170
#define YUMETARO_POWER_DECREASE_TIME			300
// Speed
#define YUMETARO_GRAVITY						0.001f
#define YUMETARO_TITLE_SCENE_GRAVITY			0.0005f
#define YUMETARO_WALKING_SPEED					0.1f 
#define YUMETARO_PREPARE_RUN_SPEED				0.15f
#define YUMETARO_RUN_SPEED						0.2f 
#define YUMETARO_JUMP_HIGH_SPEED_Y				0.02f
#define YUMETARO_JUMP_SHORT_SPEED_Y			0.27f
#define YUMETARO_JUMP_DEFLECT_SPEED			0.2f
#define YUMETARO_DIE_DEFLECT_SPEED				0.5f
#define YUMETARO_MAX_Y_SPEED					0.2f
#define YUMETARO_JUMP_LUGI_Y_SPEED				0.42f
#define YUMETARO_WAG_Y_SPEED					0.05f
#define YUMETARO_FLY_Y_SPEED					0.08f
#define YUMETARO_PIPE_Y_SPEED					0.02f
#define YUMETARO_DEFLECT_SPEED					0.4f
#define YUMETARO_JUMP_Y_SPEED					0.15f
#define YUMETARO_INCREASE_X_SPEED				0.005f
#define YUMETARO_DECREASE_X_SPEED				0.004f
#define YUMETARO_CAN_SKID_X_SPEED				0.07f
#define YUMETARO_INCREASE_RUN_X_SPEED			0.0015f
// Others
#define YUMETARO_DEFLECT_TAIL_TOP				17
#define YUMETARO_DEFLECT_TAIL_BOTTOM			2
#define YUMETARO_DEFLECT_TAIL_WIDTH			9
#define YUMETARO_NUM_BULLETS					2

class CYumetaro : public CGameObject
{
	int level;
	int untouchable;
	DWORD untouchable_start;
	bool isEndScene = false;

	int power = 0;
	int pointFactor = 0;

	float start_x;			// initial position of Mario at scene
	float start_y;
	float pipe_tele_x;
	float pipe_tele_y;

	void SetBoundingBox();
public:
	DWORD transform_start;
	DWORD run_start;
	DWORD kick_start;
	DWORD skid_start;
	DWORD tail_start;
	DWORD fire_start;
	DWORD wag_start;
	DWORD fly_start;
	DWORD fly_limit_start;
	DWORD eat_item_start;
	DWORD pipe_up_start;
	DWORD pipe_down_start;
	DWORD die_start;
	DWORD canJump_start;
	// For Title Scene
	DWORD duck_start;
	DWORD bonk_start;
	DWORD look_start;

	/*bool isPreventMoveX = false;*/
	bool isTransform = false;
	bool isOnGround = false;
	bool canJump = true;
	bool canRepeatJump = true;
	bool canJumpHigher = true;
	bool canHold = false;
	bool isHold = false;
	bool canAttack = true;
	bool canHit = true;
	bool canMultiScoreLand = false;
	bool canMultiScoreJump = false;
	bool isToPipeDown = true;
	bool isOnOtherMap = false;
	bool isHitGoomba = false;
	bool isKickKoopas = false;
	//bool canDuck = false;

	vector<CBullet*>* bullets;

	CYumetaro(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	int GetLevel() { return level; }
	int GetPower() { return power; }
	int GetPointFactor() { return pointFactor; }
	bool IsEndScene() { return isEndScene; }

	void AddPointFactor();
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	void Reset();
	void Hurt();

	// Intersert with objs
	void OnIntersect(CGameObject* obj, vector<LPGAMEOBJECT>* coObjs);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};