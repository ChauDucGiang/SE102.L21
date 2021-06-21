#include "BlackMonster.h"
#include "PlayScence.h"

CBlackMonster::CBlackMonster(float x)
{
	start_x = x;
	nx = -1;
	SetState(BLACKMONSTER_STATE_WALKING);
	vx = -BLACK_MONSTER_WALKING_SPEED;
	SetBoundingBox();
}

void CBlackMonster::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	DebugOut(L"isOnGround: %d\n", isOnGround);
	CEnemy::Update(dt, coObjects);
	if (!isActive || isDead) return;

	// Simple fall down
	if (!CGame::GetInstance()->GetCurrentScene()->GetIsObjStop())
		vy += ENEMY_GRAVITY * dt;

	if (state != BLACKMONSTER_STATE_DIE)
	{
		CYumetaro* yumetaro = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		float distanceX = x - yumetaro->x;
		// Follow Yumetaro
	
		if (abs(distanceX) >= BLACK_MONSTER_DISTANCE_MIN_X && abs(distanceX) <= BLACK_MONSTER_DISTANCE_MAX_X)
		{
			if (distanceX < 0)
			{
				nx = 1;
				vx = nx * BLACK_MONSTER_WALKING_SPEED;
			}
			else
			{
				nx = -1;
				vx = nx * BLACK_MONSTER_WALKING_SPEED;
			}
		}
				
		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;
		coEvents.clear();

		CalcPotentialCollisions(coObjects, coEvents);

		// No collision occured, proceed normally
		if (coEvents.size() == 0)
		{
			MoveThrough(OBJ_MOVE_XY);

			if (vy != 0)
			{
				isOnGround = false;				
			}
		}
		else 
		{
			MoveThrough(OBJ_MOVE_Y);

			float min_tx, min_ty, nx = 0, ny;
			float rdx = 0;
			float rdy = 0;

			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

			// Col logic with other objs	
			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];

				// Stand-Y ground
				if (ny < 0 && e->obj != NULL && !dynamic_cast<CEnemy*>(e->obj))
				{
					if (nx == 0)
					{
						MoveThrough(OBJ_MOVE_X);
					}

					/*PreventMoveY(e->obj);*/
					switch (state)
					{
					case BLACKMONSTER_STATE_WALKING:
						vy = 0;
						break;
					case BLACKMONSTER_STATE_JUMP:
						if (vy > 0)
							SetState(BLACKMONSTER_STATE_WALKING);
						break;
					default:
						break;
					}
					y = e->obj->y - (bottom - top);
				}

				// Logic with jump
				if (ny < 0 && vy >= 0)
				{
					isOnGround = true;					
				}

				if (ny == 0)
				{
					isOnGround = false;
				}

				// If isDie, do not col-X
				if (isDie)
					return;

				// Col-X
				if (e->nx != 0)
				{
					// Ground
					if (dynamic_cast<CGround*>(e->obj))
					{
						//PreventMoveX(nx, e->obj);
						SetState(BLACKMONSTER_STATE_JUMP);
					}
					//// Goomba
					//else if (dynamic_cast<CGoomba*>(e->obj))
					//{
					//	PreventMoveX(nx, e->obj);
					//	vx = -vx;
					//	e->obj->vx = -e->obj->vx;
					//}
					//// Koopas
					//else if (dynamic_cast<CKoopas*>(e->obj))
					//{
					//	CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
					//	if (koopas->state != KOOPAS_STATE_SPIN && koopas->state != KOOPAS_STATE_HOLD)
					//	{
					//		PreventMoveX(nx, e->obj);
					//		vx = -vx;
					//		koopas->vx = -koopas->vx;
					//	}
					//}
				}
			}
			// Clean up collision events
			for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
		}	

		// Intersert with obj
		/*for (int i = 0; i < coObjects->size(); i++)
		{
			if (!coObjects->at(i)->isDie)
			{
				if (this == coObjects->at(i))
					continue;
				if (AABBCheck(this, coObjects->at(i)))
				{
					OnIntersect(coObjects->at(i), coObjects);
				}
			}
		}*/
	}
}

void CBlackMonster::Render()
{
	int ani = BLACKMONSTER_ANI_WALKING;
	if (vx > 0)
		xReverse = false;
	else
		xReverse = true;
	switch (state)
	{
	case BLACKMONSTER_STATE_WALKING: 
		ani = BLACKMONSTER_ANI_WALKING;
		break;
	case BLACKMONSTER_STATE_JUMP:
		ani = BLACKMONSTER_ANI_JUMP;
		break;
	default:
		break;
	}

	animation_set->at(ani)->Render(x, y, xReverse, yReverse);
}

void CBlackMonster::SetState(int state)
{
	if (state == ENEMY_STATE_RESPAWN)
	{
		CGameObject::SetState(BLACKMONSTER_STATE_WALKING);
		vx = -((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer()->nx * BLACK_MONSTER_WALKING_SPEED;
		wait_time = GetTickCount();
		return;
	}

	CGameObject::SetState(state);
	CYumetaro* yumetaro = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	float distanceX = x - yumetaro->x;
	// Follow Yumetaro



	switch (state)
	{
	case BLACKMONSTER_STATE_WALKING:		
		vx = nx * BLACK_MONSTER_WALKING_SPEED;			
		vy = 0;
		break;
	case BLACKMONSTER_STATE_JUMP:	
		//vx = nx * vx;
		if (isOnGround)
		{
			vy = -0.25f;
		}
		break;
	case BLACKMONSTER_STATE_DIE:
		AddPoint();
		yReverse = true;
		isDie = true;
		break;
	default:
		break;
	}
}

void CBlackMonster::OnIntersect(CGameObject* obj, vector<LPGAMEOBJECT>* coObjs)
{
	// Ground
	/*if (dynamic_cast<CGround*>(obj))
	{
		PreventMoveX(nx, obj);
		PreventMoveY(obj);
	}*/
}

void CBlackMonster::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + BLACKMONSTER_BBOX_WIDTH;
	b = t + BLACKMONSTER_BBOX_HEIGHT;

	CGameObject::GetBoundingBox(l, t, r, b);
}

void CBlackMonster::SetBoundingBox()
{
	left = x;
	top = y;
	right = left + BLACKMONSTER_BBOX_WIDTH;
	bottom = top + BLACKMONSTER_BBOX_HEIGHT;
}
