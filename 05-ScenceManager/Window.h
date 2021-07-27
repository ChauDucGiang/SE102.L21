#pragma once
#pragma once
#include "GameObject.h"
#include "BlackMonster.h"
#include "Bullet.h"
#include "Yumetaro.h"
#include "PlayScence.h"

//State 
#define WINDOW_STATE_CLOSE		1
#define WINDOW_STATE_OPEN		2
#define WINDOW_STATE_ATTACK		3

// Ani

#define WINDOW_ANI_CLOSE		0
#define WINDOW_ANI_OPEN			1
#define WINDOW_ANI_ATTACK		2
// BBox
#define WINDOW_BBOX_WIDTH			48
#define WINDOW_BBOX_HEIGHT			16
#define WINDOW_ANI_SET			25
// Time
#define WINDOW_TIME_START		200

class CWindow : public CGameObject
{
	void SetBoundingBox();
	DWORD start_open;
	CBlackMonster* monster = NULL;
public:

	CWindow();
	CWindow(float x, float y);
	
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetState(int state);
};

