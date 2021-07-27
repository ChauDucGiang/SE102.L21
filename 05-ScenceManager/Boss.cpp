#include "Boss.h"
#include "QuestionBrick.h"
#include "Game.h"
#include "PlayScence.h"
#include "PSwitch.h"
#include "TitleScene.h"

CBoss::CBoss(float x)
{
	start_x = x;
	SetState(BOSS_STATE_IDLE);
	SetBoundingBox();
}

void CBoss::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CEnemy::Update(dt, coObjects);
	if (!isActive || isDead) return;
	/*UpdateDirection();*/


	// Simple fall down
	if (!CGame::GetInstance()->GetCurrentScene()->GetIsObjStop())
		vy -= ENEMY_GRAVITY * dt;
	if (state == BOSS_STATE_DIE && GetTickCount64() - die_start > BOSS_DIE_TIME)
	{
		isDead = true;
		//DeleteObjs(coObjects);
		return;
	}
	if (state != BOSS_STATE_DIE)
	{
		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;
		coEvents.clear();

		CalcPotentialCollisions(coObjects, coEvents);

		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;
		if (wait_time !=0 && GetTickCount64() - wait_time > BOSS_TIME_ATTACK) {
			wait_time = 0;
			throw_time = GetTickCount64();
			for (int i = 1; i < 5; i++) {
				monster = new CBlackMonster(x + BOSS_BBOX_WIDTH*1.5 +i*20, y);
				monster->vx = 0.02f;
				monster->vy = -0.0015;
				((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->PushBackObj(monster);
			}
			SetState(BOSS_STATE_ATTACK);
		}
		if (throw_time != 0 && GetTickCount64() - throw_time > BOSS_TIME_ATTACK)
		{
			SetState(BOSS_STATE_IDLE);
			throw_time = 0;
			wait_time = GetTickCount64();
		}
		if (monster != NULL ) {
			if(!monster->isDead)
			monster->Update(dt, coObjects);
		}
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// Col with objs		
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			// Stand-Y ground
			if (ny < 0 && e->obj != NULL && state == BOSS_STATE_WALKING)
			{
				PreventMoveY(e->obj);
			}
		}

		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}

void CBoss::Render()
{
	int ani = BOSS_ANI_IDLE;
	if (vx > 0)
		xReverse = false;
	else if (vx < 0)
		xReverse = true;
	switch (state)
	{
	case BOSS_STATE_IDLE: case BOSS_STATE_ATTACK:
		ani = BOSS_ANI_IDLE;
		break;
	default:
		break;
	}
	if (monster != NULL) {
		if(!monster->isDead)
		monster->Render();
	}
	animation_set->at(ani)->Render(x, y, xReverse, yReverse);
	RenderBoundingBox();
}

void CBoss::SetState(int state)
{
	if (state == ENEMY_STATE_RESPAWN)
	{
		CGameObject::SetState(BOSS_STATE_IDLE);
		wait_time = GetTickCount64();
		return;
	}

	CGameObject::SetState(state);

	switch (state)
	{
	case BOSS_STATE_DIE:
		//AddPoint();
		vy = 0;
		yReverse = true;
		isDie = true;
		die_start = GetTickCount64();
		break;
	case BOSS_STATE_IDLE:
		break;
	case BOSS_STATE_ATTACK:
		break;

	default:
		break;
	}

}

void CBoss::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BOSS_BBOX_WIDTH;
	bottom = y - BOSS_BBOX_HEIGHT;

	CGameObject::GetBoundingBox(left, top, right, bottom);
}


void CBoss::SetBoundingBox()
{
	left = x;
	top = y;
	right = x + BOSS_BBOX_WIDTH;
	bottom = y - BOSS_BBOX_HEIGHT;

}