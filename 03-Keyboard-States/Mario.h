#pragma once
#include "GameObject.h"

#define YUMETARO_WALKING_SPEED		0.1f
#define YUMETARO_JUMP_HIGH_SPEED_Y		0.5f
#define YUMETARO_GRAVITY			0.1f

#define YUMETARO_STATE_IDLE			0
#define YUMETARO_STATE_WALKING_RIGHT	100
#define YUMETARO_STATE_WALKING_LEFT	200
#define YUMETARO_STATE_JUMP_HIGH_LEFT			300

#define YUMETARO_ANI_IDLE_RIGHT		0
#define YUMETARO_ANI_IDLE_LEFT			1
#define YUMETARO_ANI_WALKING_RIGHT		2
#define YUMETARO_ANI_WALKING_LEFT		3

class CMario : public CGameObject
{
public: 

	void Update(DWORD dt);
	void Render();
	void SetState(int state);
};