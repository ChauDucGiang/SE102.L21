#include "TimeBomb.h"
#include "PlayScence.h"

CTimeBomb::CTimeBomb()
{
	SetState(TIME_BOMB_STATE_IDLE);
	SetBoundingBox();

}

void CTimeBomb::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	CEnemy::Update(dt, coObjects);
	if (!isActive || isDead) return;
	// Simple fall down
	if (!CGame::GetInstance()->GetCurrentScene()->GetIsObjStop() && state == TIME_BOMB_STATE_FALL)
	{

		vy -= ENEMY_GRAVITY * dt;
		MoveThrough(OBJ_MOVE_XY);
	}
	if (state == TIME_BOMB_STATE_EXPLODE && GetTickCount64() - die_start > TIME_BOMB_DIE_TIME) {
		isDead = true;
		//DeleteObjs(coObjects);
		return;
	}
	if (state != TIME_BOMB_STATE_DIE)
	{
		CYumetaro* yumetaro = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		float distanceX = x - yumetaro->x;
		if (distanceX < 20 && distanceX > -20 && state==TIME_BOMB_STATE_IDLE) {
			
			SetState(TIME_BOMB_STATE_FALL);
			
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

		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// Col logic with other objs	
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (ny > 0 && e->obj != NULL && !dynamic_cast<CEnemy*>(e->obj) && state == TIME_BOMB_STATE_FALL && !dynamic_cast<CSlopeBrick*>(e->obj))
			{
			// Stand-Y ground
				SetState(TIME_BOMB_STATE_EXPLODE);
				
				//DeleteObjs(coObjects);
				PreventMoveY(e->obj);
			}
		}
		// Clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}

void CTimeBomb::Render()
{
	int ani = TIME_BOMB_ANI_IDLE;
	switch (state) {
	case TIME_BOMB_STATE_IDLE:
		ani = TIME_BOMB_ANI_IDLE;
		break;
	case TIME_BOMB_STATE_FALL:
		ani = TIME_BOMB_ANI_FALL;
		break;
	case TIME_BOMB_STATE_EXPLODE:
		ani = TIME_BOMB_ANI_EXPLODE;
	default:
		break;
	}
	animation_set->at(ani)->Render(x, y, xReverse, yReverse);
	RenderBoundingBox();
}

void CTimeBomb::SetState(int state)
{
	if (state == ENEMY_STATE_RESPAWN)
	{
		CGameObject::SetState(TIME_BOMB_STATE_IDLE);
		/*vx = -((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer()->nx * BLACK_MONSTER_WALKING_SPEED;
		wait_time = GetTickCount64();*/
		//wait_time = GetTickCount64();
		return;
	}

	CGameObject::SetState(state);
	switch (state) {
	case TIME_BOMB_STATE_IDLE:
		vy = 0;
		break;
	case TIME_BOMB_STATE_FALL:
		vy = 0.02f;
		break;
	case TIME_BOMB_STATE_EXPLODE:
		die_start = GetTickCount64();
		break;
	default:
		break;
	}

}

void CTimeBomb::OnIntersect(CGameObject* obj, vector<LPGAMEOBJECT>* coObjs)
{
	/*if (dynamic_cast<CSlopeBrick*>(obj)) {
		SetState(TIME_BOMB_STATE_EXPLODE);
		PreventMoveY(obj);
	}*/
}

void CTimeBomb::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + TIME_BOMB_BBOX_WIDTH;
	b = y - TIME_BOMB_BBOX_HEIGHT;

	CGameObject::GetBoundingBox(l, t, r, b);
}

void CTimeBomb::SetBoundingBox()
{
	left = x;
	top = y;
	right = left + TIME_BOMB_BBOX_WIDTH;
	bottom = top - TIME_BOMB_BBOX_HEIGHT;
}

