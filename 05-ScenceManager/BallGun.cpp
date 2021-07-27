#include "BallGun.h"
#include "PlayScence.h"

CBallGun::CBallGun()
{
	SetBoundingBox();

}

void CBallGun::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	
	CEnemy::Update(dt, coObjects);
	if (!isActive || isDead) return;
	if (!CGame::GetInstance()->GetCurrentScene()->GetIsObjStop())
	{
		vy -= ENEMY_GRAVITY * dt;
		MoveThrough(OBJ_MOVE_XY);
	}
	if (state != BALL_GUN_STATE_DIE)
	{
		// Throw 1
		if (wait_time != 0 && GetTickCount64() - wait_time > BALL_GUN_WAIT_TIME )
		{			
			SetState(BALL_GUN_STATE_ATTACK);
			wait_time = 0;
			throw_time = GetTickCount64();
			CBullet* bullet = new CBullet();
			//bullet->isEnemy = true;
			((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->PushBackObj(bullet);
			if (nx < 0)
			{
				bullet->SetPosition(right, y);
			}
			else
			{
				bullet->SetPosition(right, y);
			}
			bullet->vx = nx * (BULLET_ENEMY_X_SPEED + 0.015);
			//bullet->vy = BULLET_ENEMY_NEAR_Y_SPEED;
			bullet->SetState(BULLET_STATE_FIRE);
		}
		// Throw 2
		if (throw_time != 0 && GetTickCount64() - throw_time > BALL_GUN_WAIT_TIME)
		{
			SetState(BALL_GUN_STATE_IDLE);
			throw_time = 0;
			wait_time = GetTickCount64();
		}
		for (int i = 0; i < coObjects->size(); i++)
		{
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

			// Stand-Y ground
			if (ny > 0 && e->obj != NULL && !dynamic_cast<CEnemy*>(e->obj) && !dynamic_cast<CSlopeBrick*>(e->obj))
			{
				vx = 0;
				PreventMoveY(e->obj);
			}

			if (dynamic_cast<CSlopeBrick*>(e->obj))
			{
				
				if (ny < 0) {
					vx = -0.05;
				}
				else if (ny > 0) {
					vx = 0.05;
				}

				isColSlopeBrick = true;
				MoveThrough(OBJ_MOVE_XY);
				if (e->nx != 0)
					x += dx;
				else if (e->ny != 0)
					y += dy;
			}
			if (nx < 0 && dynamic_cast<CGround*>(e->obj)) {
				vy += ENEMY_GRAVITY * dt;
				vx = 0;
			}
			// Col-X
			/*if (nx == 0 )
			{
				vx = 0;
			}
			else if (nx < 0 && dynamic_cast<CGround*>(e->obj)) {
				vy += ENEMY_GRAVITY * dt;
				vx = 0;
			}*/
		}
		// Clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}

void CBallGun::Render()
{
	int ani = BALL_GUN_ANI_IDLE;
	switch (state) {
		case BALL_GUN_STATE_IDLE:
			ani = BALL_GUN_ANI_IDLE;
			break;	
		case BALL_GUN_STATE_ATTACK:
			ani = BALL_GUN_ANI_ATTACK;
			break;
		default:
			break;
	}
	animation_set->at(ani)->Render(x, y, xReverse, yReverse);
	RenderBoundingBox();
}

void CBallGun::SetState(int state)
{
	if (state == ENEMY_STATE_RESPAWN)
	{
		CGameObject::SetState(BALL_GUN_STATE_IDLE);
		/*vx = -((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer()->nx * BLACK_MONSTER_WALKING_SPEED;
		wait_time = GetTickCount64();*/
		wait_time = GetTickCount64();
		return;
	}

	CGameObject::SetState(state);
	//switch (state)
	//{
	//caseBALL_GUN_STATE_WALKING:
	//	vx = nx * BLACK_MONSTER_WALKING_SPEED;
	//	vy = 0;
	//	break;
	//caseBALL_GUN_STATE_JUMP:

	//	if (isOnGround)
	//	{
	//		vy = -0.25f;
	//	}
	//	break;
	//caseBALL_GUN_STATE_DIE:
	//	AddPoint();
	//	yReverse = true;
	//	isDie = true;
	//	break;
	//default:
	//	break;
	//}
}

void CBallGun::OnIntersect(CGameObject* obj, vector<LPGAMEOBJECT>* coObjs)
{
	// Ground
	/*if (dynamic_cast<CGround*>(obj))
	{
		PreventMoveX(nx, obj);
		PreventMoveY(obj);
	}*/
}

void CBallGun::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BALL_GUN_BBOX_WIDTH;
	b = y - BALL_GUN_BBOX_HEIGHT;

	CGameObject::GetBoundingBox(l, t, r, b);
}

void CBallGun::SetBoundingBox()
{
	left = x;
	top = y;
	right = left + BALL_GUN_BBOX_WIDTH;
	bottom = top - BALL_GUN_BBOX_HEIGHT;
}
