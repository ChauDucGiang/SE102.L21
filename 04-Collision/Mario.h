#pragma once
#include "GameObject.h"

#define YUMETARO_WALKING_SPEED		0.1f 
//0.1f
#define YUMETARO_JUMP_HIGH_SPEED_Y		0.5f
#define YUMETARO_JUMP_DEFLECT_SPEED 0.2f
#define YUMETARO_GRAVITY			0.002f
#define YUMETARO_DIE_DEFLECT_SPEED	 0.5f

#define YUMETARO_STATE_IDLE			0
#define YUMETARO_STATE_WALKING_RIGHT	100
#define YUMETARO_STATE_WALKING_LEFT	200
#define YUMETARO_STATE_JUMP_HIGH_LEFT			300
#define YUMETARO_STATE_DIE				400

#define YUMETARO_ANI_BIG_IDLE_RIGHT		0
#define YUMETARO_ANI_BIG_IDLE_LEFT			1
#define YUMETARO_ANI_SMALL_IDLE_RIGHT		2
#define YUMETARO_ANI_SMALL_IDLE_LEFT			3

#define YUMETARO_ANI_BIG_WALKING_RIGHT			4
#define YUMETARO_ANI_BIG_WALKING_LEFT			5
#define YUMETARO_ANI_SMALL_WALKING_RIGHT		6
#define YUMETARO_ANI_SMALL_WALKING_LEFT		7

#define YUMETARO_ANI_DIE				8

#define	YUMETARO_LEVEL_SMALL	1
#define	YUMETARO_LEVEL_BIG		2

#define YUMETARO_BIG_BBOX_WIDTH  15
#define YUMETARO_BIG_BBOX_HEIGHT 27

#define YUMETARO_SMALL_BBOX_WIDTH  13
#define YUMETARO_SMALL_BBOX_HEIGHT 15

#define YUMETARO_UNTOUCHABLE_TIME 5000


class CMario : public CGameObject
{
	int level;
	int untouchable;
	DWORD untouchable_start;
public: 
	CMario() : CGameObject()
	{
		level = YUMETARO_LEVEL_BIG;
		untouchable = 0;
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};