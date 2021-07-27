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
CBlackMonster::CBlackMonster(float x, float y) {
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(BLACKMONSTER_ANI_SET);
	SetAnimationSet(ani_set);
	SetRespawnPosition(x, y);
	SetPosition(x, y);
	start_x = x;
	isOnWindow = true;
	nx = -1;
	SetState(BLACKMONSTER_STATE_WALKING);
	vx = -BLACK_MONSTER_WALKING_SPEED/5;
	SetBoundingBox();
}
void CBlackMonster::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	/*DebugOut(L"isOnGround: %d\n", isOnGround);*/
	CEnemy::Update(dt, coObjects);
	if (!isActive || isDead) return;

	// Simple fall down
	if (!CGame::GetInstance()->GetCurrentScene()->GetIsObjStop())
		vy -= ENEMY_GRAVITY * dt;
	if (state == BLACKMONSTER_STATE_DIE_REVERSE && GetTickCount64() - die_start > BLACKMONSTER_DIE_TIME)
	{
		isDead = true;
		DeleteObjs(coObjects);
		return;
	}
	if (state != BLACKMONSTER_STATE_DIE_REVERSE)
	{
		CYumetaro* yumetaro = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		float distanceX = x - yumetaro->x;
		// Follow Yumetaro
	
		if (abs(distanceX) >= BLACK_MONSTER_DISTANCE_MIN_X && abs(distanceX) <= BLACK_MONSTER_DISTANCE_MAX_X)
		{
			if (distanceX < 0)
			{
				nx = 1;
				if (isOnWindow) {
					vx = nx * BLACK_MONSTER_WALKING_SPEED/5;
				}
				else {
					vx = nx * BLACK_MONSTER_WALKING_SPEED;
				}
			
			}
			else
			{
				nx = -1;
				if (isOnWindow) {
					vx = nx * BLACK_MONSTER_WALKING_SPEED / 5;
				}
				else {
					vx = nx * BLACK_MONSTER_WALKING_SPEED;
				}
			
			}
		}

		for (UINT i = 0; i < coObjects->size(); i++)
		{
			if (AABBCheck(this, coObjects->at(i)))
			{
				if (dynamic_cast<CSlopeBrick*>(coObjects->at(i)))
				{
					CSlopeBrick* brick = dynamic_cast<CSlopeBrick*>(coObjects->at(i));
					brick->Collision(this, dy, dx);
				}
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
				if (!isColSlopeBrick) {
					isOnGround = false;
				}
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
				if (ny > 0 && e->obj != NULL && !dynamic_cast<CEnemy*>(e->obj) && !dynamic_cast<CSlopeBrick*>(e->obj))
				{
					if (nx == 0)
					{
						MoveThrough(OBJ_MOVE_X);
					}
					if (dynamic_cast<CCarousel*>(e->obj))
					{
						CCarousel* carousel = dynamic_cast<CCarousel*>(e->obj);
						if (carousel->GetState() != NULL) {
							x += carousel->GetBrickSpeed() * dt;
						}
					}
					/*PreventMoveY(e->obj);*/
					switch (state)
					{
					case BLACKMONSTER_STATE_WALKING:
						vy = 0;
						break;
					case BLACKMONSTER_STATE_JUMP:
						if (vy < 0)
							SetState(BLACKMONSTER_STATE_WALKING);
						break;
					default:
						break;
					}
				
					y = e->obj->y + (top - bottom);
				}
				else if (dynamic_cast<CSlopeBrick*>(e->obj))
				{
					/*vy = 0;*/
					isColSlopeBrick = true;
					MoveThrough(OBJ_MOVE_XY);
					if (e->nx != 0)
						x += dx;
					else if (e->ny != 0)
						y += dy;
				}
				else if (dynamic_cast<CEnemy*>(e->obj)) {
					MoveThrough(OBJ_MOVE_X);
				}
				else if (dynamic_cast<CStar*>(e->obj)) {
					MoveThrough(OBJ_MOVE_XY);
				}
				if (e->nx != 0)
				{
						// Ground
						if (dynamic_cast<CGround*>(e->obj))
						{
							//PreventMoveX(nx, e->obj);
							SetState(BLACKMONSTER_STATE_JUMP);
						}
						else if (dynamic_cast<CStar*>(e->obj)) {
							MoveThrough(OBJ_MOVE_XY);
						}

				}

				// Logic with jump
				if (ny > 0 && vy == 0)
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
			}
			// Clean up collision events
			for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
		}	
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
		/*vx = -((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer()->nx * BLACK_MONSTER_WALKING_SPEED;
		wait_time = GetTickCount64();*/
		return;
	}

	CGameObject::SetState(state);
	switch (state)
	{
	case BLACKMONSTER_STATE_WALKING:	
		if (isOnWindow) {
			vx = nx * BLACK_MONSTER_WALKING_SPEED / 5;
			vy = 0;
		}
		else {
			vx = nx * BLACK_MONSTER_WALKING_SPEED;
			vy = 0;
		}
		break;
	case BLACKMONSTER_STATE_JUMP:	
	
		if (isOnGround)
		{
			vy = 0.25f;
		}
		break;
	case BLACKMONSTER_STATE_DIE_REVERSE:
		AddPoint();
		vx = 0;
		vy = 0;
		yReverse = true;
		isDie = true;
		die_start = GetTickCount64();
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
	b = t - BLACKMONSTER_BBOX_HEIGHT;

	CGameObject::GetBoundingBox(l, t, r, b);
}

void CBlackMonster::SetBoundingBox()
{
	left = x;
	top = y;
	right = left + BLACKMONSTER_BBOX_WIDTH;
	bottom = top - BLACKMONSTER_BBOX_HEIGHT;
}
