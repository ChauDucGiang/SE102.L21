#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy += YUMETARO_GRAVITY*dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state!=YUMETARO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount() - untouchable_start > YUMETARO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision occured, proceed normally
	if (coEvents.size()==0)
	{
		x += dx; 
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block 
		x += min_tx*dx + nx*0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty*dy + ny*0.4f;
		
		if (nx!=0) vx = 0;
		if (ny!=0) vy = 0;

		// Collision logic with Goombas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CGoomba *>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (goomba->GetState()!= GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						vy = -YUMETARO_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable==0)
					{
						if (goomba->GetState()!=GOOMBA_STATE_DIE)
						{
							if (level > YUMETARO_LEVEL_SMALL)
							{
								level = YUMETARO_LEVEL_SMALL;
								StartUntouchable();
							}
							else 
								SetState(YUMETARO_STATE_DIE);
						}
					}
				}
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CMario::Render()
{
	int ani;
	if (state == YUMETARO_STATE_DIE)
		ani = YUMETARO_ANI_DIE;
	else
	if (level == YUMETARO_LEVEL_BIG)
	{
		if (vx == 0)
		{
			if (nx>0) ani = YUMETARO_ANI_BIG_IDLE_RIGHT;
			else ani = YUMETARO_ANI_BIG_IDLE_LEFT;
		}
		else if (vx > 0) 
			ani = YUMETARO_ANI_BIG_WALKING_RIGHT; 
		else ani = YUMETARO_ANI_BIG_WALKING_LEFT;
	}
	else if (level == YUMETARO_LEVEL_SMALL)
	{
		if (vx == 0)
		{
			if (nx>0) ani = YUMETARO_ANI_SMALL_IDLE_RIGHT;
			else ani = YUMETARO_ANI_SMALL_IDLE_LEFT;
		}
		else if (vx > 0)
			ani = YUMETARO_ANI_SMALL_WALKING_RIGHT;
		else ani = YUMETARO_ANI_SMALL_WALKING_LEFT;
	}

	int alpha = 255;
	if (untouchable) alpha = 128;
	animations[ani]->Render(x, y, alpha);

	RenderBoundingBox();
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
		vy = -YUMETARO_JUMP_HIGH_SPEED_Y;
	case YUMETARO_STATE_IDLE: 
		vx = 0;
		break;
	case YUMETARO_STATE_DIE:
		vy = -YUMETARO_DIE_DEFLECT_SPEED;
		break;
	}
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y; 

	if (level==YUMETARO_LEVEL_BIG)
	{
		right = x + YUMETARO_BIG_BBOX_WIDTH;
		bottom = y + YUMETARO_BIG_BBOX_HEIGHT;
	}
	else
	{
		right = x + YUMETARO_SMALL_BBOX_WIDTH;
		bottom = y + YUMETARO_SMALL_BBOX_HEIGHT;
	}
}

