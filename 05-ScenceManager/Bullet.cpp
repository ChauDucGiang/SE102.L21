#include "Bullet.h"
#include "PlayScence.h"

CBullet::CBullet()
{
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(BULLET_ANI_SET);
	SetAnimationSet(ani_set);

	Dead();

	SetBoundingBox();
}

void CBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	DebugOut(L"[Bullet vx]: %f\n", vx);
	CGameObject::Update(dt, coObjects);
	UpdateDirection();

	MoveThrough(OBJ_MOVE_XY);
	
	CYumetaro* yumetaro = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	float distanceX = abs((x + BULLET_BBOX_WIDTH / 2) - (yumetaro->x + YUMETARO_BBOX_WIDTH / 2));
	float distanceY = ((y + BULLET_BBOX_HEIGHT / 2) - (yumetaro->y + YUMETARO_BBOX_HEIGHT / 2));

	// Get others instance
	CCamera* camera = CCamera::GetInstance();
	int leftMap = camera->GetLeftMap();
	int topMap = camera->GetTopMap();
	int rightMap = camera->GetRightMap();
	int bottomMap = camera->GetBottomMap();
	int heightMap = camera->GetHeightMap();

	// Bullet dead when col edge map or far from Mario
	/*if (distanceX > SCREEN_WIDTH || distanceY > SCREEN_HEIGHT || x < leftMap || x + BULLET_BBOX_WIDTH > rightMap || y < topMap || y > topMap + heightMap)
	{
	
		if (isEnemy)
			DeleteFrontObjs(coObjects);
	}*/
	
	// If not bullet of Piranha
	if (!isEnemy)
	{
		if (state == BULLET_STATE_EXPLODE)
		{
			if (GetTickCount64() - explode_start > BULLET_EXPLODE_TIME) {
				/*Dead();*/
				DeleteObjs(coObjects);
			}
			return;
		}
		
		// Simple fall down
		if (!CGame::GetInstance()->GetCurrentScene()->GetIsObjStop())
			vy -= 2 * BULLET_GRAVITY * dt;				
		
		if (state != BULLET_STATE_EXPLODE)
		{
			// Intersert with objs
			for (int i = 0; i < coObjects->size(); i++)
			{
				if (dynamic_cast<CBullet*>(coObjects->at(i)))
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
			if(state ==  BULLET_STATE_FIRE && GetTickCount64() - die_start > BULLET_DIE_TIME)
			{
				SetState(BULLET_STATE_EXPLODE);
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
					PreventMoveY(e->obj);
					if (dynamic_cast<CGround*>(e->obj)) {
						if (vx >= 0.015f) vx -= 0.0025f;
						if (vx <= -0.015f) vx += 0.0025f;
					}
				}
				else if (dynamic_cast<CSlopeBrick*>(e->obj))
				{
					/*vy = 0;*/
					isColSlopeBrick = true;
					//vx += 0.001f;
					/*if (e->nx != 0)
						vx += 0.001 * dt;
					else if (e->ny != 0)
						y += dy;*/
				}
				if (dynamic_cast<CWater*>(e->obj) && state == BULLET_STATE_FIRE) {
					MoveThrough(OBJ_MOVE_Y);
					
				}
			}

			// clean up collision events
			for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
		}
	}		
}

void CBullet::Render()
{
	int ani = BULLET_ANI_FIRE;
	switch (state) {
	case BULLET_STATE_FIRE:
		ani = BULLET_ANI_FIRE;
		break;
	case BULLET_STATE_EXPLODE:
		ani = BULLET_ANI_EXPLODE;
		break;
	default:
		break;

	}
	animation_set->at(ani)->Render(x, y, xReverse, yReverse);

	//RenderBoundingBox();
}

void CBullet::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{	
	case BULLET_STATE_FIRE:
		isDie = false;
		isDead = false;
		die_start = GetTickCount64();
		break;
	case BULLET_STATE_EXPLODE:
		vx = 0;
		vy = 0;
		isDie = true;
		explode_start = GetTickCount64();
		break;		
	}
}

void CBullet::OnIntersect(CGameObject* obj, vector<LPGAMEOBJECT>* coObjs)
{
	if (!obj->isDie) {
		if (dynamic_cast<CWater*>(obj)) {
			SetState(BULLET_STATE_EXPLODE);
		}
	}
}

void CBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BULLET_BBOX_WIDTH;
	bottom = y - BULLET_BBOX_HEIGHT;

	CGameObject::GetBoundingBox(left, top, right, bottom);
}

void CBullet::SetBoundingBox()
{
	left = x;
	top = y;
	right = x + BULLET_BBOX_WIDTH;
	bottom = y - BULLET_BBOX_HEIGHT;
}
