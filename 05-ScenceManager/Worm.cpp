#include "Worm.h"
#include "QuestionBrick.h"
#include "Game.h"
#include "PlayScence.h"
#include "PSwitch.h"
#include "TitleScene.h"

CWorm::CWorm(float x)
{
	start_x = x;
	nx = 1;
	SetState(WORM_STATE_WALKING);
	//vx = -WORM_WALKING_SPEED;
	SetBoundingBox();
}

void CWorm::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CEnemy::Update(dt, coObjects);
	MoveThrough(OBJ_MOVE_XY);

	if (!isActive || isDead) return;
	/*UpdateDirection();*/

	if (state == WORM_STATE_DIE && GetTickCount() - die_start > WORM_DIE_TIME)
	{
		isDead = true;
		DeleteObjs(coObjects);
		return;
	}
	// Simple fall down
	if (!CGame::GetInstance()->GetCurrentScene()->GetIsObjStop())
		vy -= ENEMY_GRAVITY * dt;
	
	if (state != WORM_STATE_DIE)
	{	
		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;
		coEvents.clear();

		CalcPotentialCollisions(coObjects, coEvents);

		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// Col with objs		
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];			
		
			// Stand-Y ground
		if (ny > 0 && e->obj != NULL && state == WORM_STATE_WALKING)
			{
				//// Case Mario hit Brick to hit Worm
				//if (dynamic_cast<CBrick*>(e->obj))
				//{
				//	CBrick* brick = dynamic_cast<CBrick*>(e->obj);
				//	if (brick->isUp)
				//	{
				//		yReverse = true;
				//		vx = mario->nx * WORM_DIE_X_SPEED;
				//		vy = -WORM_HIT_Y_SPEED;
				//		SetState(WORM_STATE_HIDE);
				//	}
				//}

				// Red Worm do not leave the platform
				if (dynamic_cast<CGround*>(e->obj))
				{
					CGround* ground = dynamic_cast<CGround*>(e->obj);

					PreventMoveY(ground);
					
					if (left < e->obj->left - WORM_BBOX_WIDTH / 10)
					{
						x = e->obj->left - WORM_BBOX_WIDTH / 10;
						vx = -vx;
					}
					if (right > e->obj->right + WORM_BBOX_WIDTH / 10)
					{
						x = e->obj->right - 0.91 * WORM_BBOX_WIDTH;						
						vx = -vx;
					}
				}				
			}
		}

		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}

void CWorm::Render()
{
	int ani = WORM_ANI_WALKING;
	if (vx > 0)
		xReverse = false;
	else if (vx < 0)
		xReverse = true;
	switch (state)
	{
	case WORM_STATE_WALKING:
		ani = WORM_ANI_WALKING;
		break;
	default:
		break;
	}
	animation_set->at(ani)->Render(x, y, xReverse, yReverse);
	RenderBoundingBox();
}

void CWorm::SetState(int state)
{
	if (state == ENEMY_STATE_RESPAWN)
	{
		CGameObject::SetState(WORM_STATE_WALKING);
		return;
	}

	CGameObject::SetState(state);

	switch (state)
	{
	case WORM_STATE_DIE:
		vx = 0;
		vy = 0;
		yReverse = true;
		isDie = true;
		die_start = GetTickCount64();
		break;
	case WORM_STATE_WALKING:
		//yReverse = false;
		vx = WORM_WALKING_SPEED;
		break;
	default:
		break;
	}

}

void CWorm::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + WORM_BBOX_WIDTH;
	bottom = y - WORM_BBOX_HEIGHT;

	CGameObject::GetBoundingBox(left, top, right, bottom);
}


void CWorm::SetBoundingBox()
{
	left = x;
	top = y;
	right = x + WORM_BBOX_WIDTH;
	bottom = y - WORM_BBOX_HEIGHT;
	
}