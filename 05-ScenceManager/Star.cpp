#include "Star.h"
#include "PlayScence.h"

CStar::CStar()
{
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(STAR_ANI_SET);
	SetAnimationSet(ani_set);

	Dead();

	SetBoundingBox();
}

void CStar::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	UpdateDirection();

	MoveThrough(OBJ_MOVE_XY);

	CYumetaro* yumetaro = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	
	if (state == STAR_STATE_EXPLODE)
	{
		if (GetTickCount64() - explode_start > STAR_EXPLODE_TIME)
		{
			vx = 0;
			vy = 0;
			yumetaro->canAttack = true;
			isFirstLand = false;
			DeleteObjs(coObjects);
		}

		return;
	}

	// Simple fall down
	if (!CGame::GetInstance()->GetCurrentScene()->GetIsObjStop())
		vy -= STAR_GRAVITY * dt;

	if (state != STAR_STATE_EXPLODE)
	{
		// Intersert with objs
		for (int i = 0; i < coObjects->size(); i++)
		{
			if (dynamic_cast<CStar*>(coObjects->at(i)))
				continue;
			if (AABBCheck(this, coObjects->at(i)))
			{
				if (dynamic_cast<CSlopeBrick*>(coObjects->at(i)))
				{
					CSlopeBrick* brick = dynamic_cast<CSlopeBrick*>(coObjects->at(i));
					brick->Collision(this, dy, dx);
				}

				OnIntersect(coObjects->at(i), coObjects);
			}
		}
		if (state == STAR_STATE_FIRE && GetTickCount64() - die_start > STAR_DIE_TIME)
		{
			SetState(STAR_STATE_EXPLODE);
		}
		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		coEvents.clear();

		CalcPotentialCollisions(coObjects, coEvents);

		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		colX = nx;

		// Collision logic with other objects			
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (ny > 0 && e->obj != NULL && !dynamic_cast<CEnemy*>(e->obj) && !dynamic_cast<CSlopeBrick*>(e->obj) && !dynamic_cast<CWater*>(e->obj))
			{
				
				if (!isFirstLand)
				{
					vy = 0.1 * y / firstY;
					isFirstLand = true;
				}
				else
				{
					vy = (abs(vy) + 0.02f);
				}
				y = e->obj->y + (top - bottom);
			}
			//else if (dynamic_cast<CBoss*>(e->obj))
			//{
			//	
			//	if (nx != 0 || ny != 0) {
			//		SetState(STAR_STATE_EXPLODE);
			//		CBoss* boss = dynamic_cast<CBoss*>(e->obj);
			//		
			//		boss->vx = nx * ENEMY_DIE_X_SPEED;
			//		boss->vy = -ENEMY_DIE_Y_SPEED;
			//		boss->SetState(BOSS_STATE_DIE);
			//		boss->hp_boss -= 1;
			//	}
			//
			//	//boss->SetState(WORM_STATE_DIE);
			//}
			// Hit head and col-X
			
			else  if (dynamic_cast<CGround*>(e->obj))
			{

				if (nx != 0)
				{
					vx = -vx;
				}
				
			}
			else if (ny < 0) {
				if (!isFirstLand)
				{
					vy = -0.1 * y / firstY;
					isFirstLand = true;
				}
				else
				{
					vy = -(abs(vy) - 0.02f);
				}
			}
			
		}

		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}

void CStar::Render()
{
	int ani = STAR_ANI_FIRE;
	switch (state) {
	case STAR_STATE_FIRE:
		ani = STAR_ANI_FIRE;
		break;
	case STAR_STATE_EXPLODE:
		ani = STAR_ANI_EXPLODE;
		break;
	default:
		break;

	}
	animation_set->at(ani)->Render(x, y, xReverse, yReverse);

	//RenderBoundingBox();
}

void CStar::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case STAR_STATE_FIRE:
		isDie = false;
		isDead = false;
		die_start = GetTickCount64();
		break;
	case STAR_STATE_EXPLODE:
		vx = 0;
		vy = 0;
		isDie = true;
		explode_start = GetTickCount64();
		break;
	}
}

void CStar::OnIntersect(CGameObject* obj, vector<LPGAMEOBJECT>* coObjs)
{
	if (!obj->isDie)
	{
		if (dynamic_cast<CBlackMonster*>(obj))
		{
			CBlackMonster* enemy = dynamic_cast<CBlackMonster*>(obj);
			enemy->vx = nx * ENEMY_DIE_X_SPEED;
			enemy->vy = -ENEMY_DIE_Y_SPEED;
			enemy->SetState(BLACKMONSTER_STATE_DIE_REVERSE);
		}
		if (dynamic_cast<CWorm*>(obj))
		{
			CWorm* worm = dynamic_cast<CWorm*>(obj);
			worm->vx = nx * ENEMY_DIE_X_SPEED;
			worm->vy = -ENEMY_DIE_Y_SPEED;
			worm->SetState(WORM_STATE_DIE);
		}
		if (dynamic_cast<CBoss*>(obj))
		{
			SetState(STAR_STATE_EXPLODE);
			CBoss* boss = dynamic_cast<CBoss*>(obj);

			boss->vx = nx * ENEMY_DIE_X_SPEED;
			boss->vy = -ENEMY_DIE_Y_SPEED;
			boss->SetState(BOSS_STATE_DIE);
			boss->hp_boss -= 1;
		}
	}
}

void CStar::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + STAR_BBOX_WIDTH;
	bottom = y - STAR_BBOX_HEIGHT;

	CGameObject::GetBoundingBox(left, top, right, bottom);
}

void CStar::SetBoundingBox()
{
	left = x;
	top = y;
	right = x + STAR_BBOX_WIDTH;
	bottom = y - STAR_BBOX_HEIGHT;
}
