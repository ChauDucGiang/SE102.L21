#include "Mario.h"

void CMario::Update(DWORD dt)
{
	CGameObject::Update(dt);

	// simple fall down
	vy += YUMETARO_GRAVITY;
	if (y > 100) 
	{
		vy = 0; y = 100.0f;
	}

	// simple screen edge collision!!!
	if (vx > 0 && x > 290) x = 290;
	if (vx < 0 && x < 0) x = 0;
}

void CMario::Render()
{
	int ani;
	if (vx == 0)
	{
		if (nx>0) ani = YUMETARO_ANI_IDLE_RIGHT;
		else ani = YUMETARO_ANI_IDLE_LEFT;
	}
	else if (vx > 0) 
		ani = YUMETARO_ANI_WALKING_RIGHT; 
	else ani = YUMETARO_ANI_WALKING_LEFT;

	animations[ani]->Render(x, y);
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case YUMETARO_STATE_WALKING_RIGHT:
		vx = YUMETARO_WALKING_SPEED;
		nx = 1;
		break;
	case YUMETARO_STATE_WALKING_LEFT: 
		vx = -YUMETARO_WALKING_SPEED;
		nx = -1;
		break;
	case YUMETARO_STATE_JUMP_HIGH_LEFT: 
		if (y==100)
			vy = -YUMETARO_JUMP_HIGH_SPEED_Y;

	case YUMETARO_STATE_IDLE: 
		vx = 0;
		break;
	}
}

