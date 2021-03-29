#include <algorithm>
#include <assert.h>
#include "Yumetaro.h"
#include "Utils.h"
#include "Game.h"
#include "Portal.h"
#include "PlayScence.h"
#include "PlayerInfo.h"
#include "Point.h"
#include "Enemy.h"

CYumetaro::CYumetaro(float x, float y) : CGameObject()
{
	untouchable = 0;
	SetState(YUMETARO_STATE_IDLE);

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;

	bullets = new vector<CBullet*>();
	for (int i = 0; i < YUMETARO_NUM_BULLETS; i++)
	{
		CBullet* bullet = new CBullet();
		//bullet->SetAnimationSet(ani_set);
		bullets->push_back(bullet);
		CGame::GetInstance()->GetCurrentScene()->GetFrontObjs()->push_back(bullet);
	}

	SetBoundingBox();
}

void CYumetaro::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Get others instance
	CCamera* camera = CCamera::GetInstance();
	int leftMap = camera->GetLeftMap();
	int topMap = camera->GetTopMap();
	int rightMap = camera->GetRightMap();
	int bottomMap = camera->GetBottomMap();
	int heightMap = camera->GetHeightMap();
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();

	CGameObject::Update(dt);

	// Simple fall down
	if (!CGame::GetInstance()->GetCurrentScene()->GetIsObjStop())
	{
		if (isOnTitleScene)
			vy += YUMETARO_TITLE_SCENE_GRAVITY * dt;
		else
			vy += YUMETARO_GRAVITY * dt;
	}

	// Max jump
	if (!isOnTitleScene && vy <= -YUMETARO_MAX_Y_SPEED)
	{
		vy = -YUMETARO_MAX_Y_SPEED;
		canJumpHigher = false;
	}

	// Die
	if (state == YUMETARO_STATE_DIE)
	{
		MoveThrough(OBJ_MOVE_XY);

		if (GetTickCount() - die_start > YUMETARO_DIE_TIME)
		{
			CPlayerInfo::GetInstance()->AdjustLife(-LIFE_1);
			CGame::GetInstance()->SwitchScene(WORLD_MAP_1);
		}
		return;
	}

	// yumetaro when col edge map
	if (x <= leftMap)
		x = leftMap;
	if (y <= topMap)
		y = topMap;
	if (x + YUMETARO_BBOX_WIDTH >= rightMap)
		x = rightMap - YUMETARO_BBOX_WIDTH;

	// Intersert with objs
	for (int i = 0; i < coObjects->size(); i++)
	{
		if (AABBCheck(this, coObjects->at(i)))
		{
			OnIntersect(coObjects->at(i), coObjects);
		}
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// Turn off collision when die 
	if (state != YUMETARO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// Reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > YUMETARO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		MoveThrough(OBJ_MOVE_XY);

		if (vy != 0)
		{
			isOnGround = false;
			if (vy > 0)
				canJump = false;
		}
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
		colX = nx;

		x += min_tx * dx + nx * YUMETARO_DEFLECT_SPEED;
		y += min_ty * dy + ny * YUMETARO_DEFLECT_SPEED;

		// Col with objs
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			e->obj->colX = e->nx;
			e->obj->colY = e->ny;

			// Stand on obj
			if (ny < 0 && e->obj != NULL)
			{
				// Disable mutiScore
				if (canMultiScoreJump && isOnGround)
				{
					canMultiScoreJump = false;
					pointFactor = 0;
				}
				// Stand-Y
				if (state != YUMETARO_STATE_DIE)
					PreventMoveY(e->obj);
			}

			// Logic with jump
			if (ny < 0 && vy >= 0)
			{
				isOnGround = true;
				if (canRepeatJump)
				{
					canJump = true;
					canJumpHigher = true;
				}
				else
				{
					canJump = false;
					canJumpHigher = false;
				}
			}
			// Head hit objs
			else if (ny > 0 && !dynamic_cast<CBullet*>(e->obj))
			{
				vy = 0;
				canJump = false;
				canJumpHigher = false;
			}

			if (dynamic_cast<CEnemy*>(e->obj))
			{
				canJump = true;
				canJumpHigher = true;
			}

			// Bullet
			else if (dynamic_cast<CBullet*>(e->obj))
			{
				MoveThrough(OBJ_MOVE_XY);

				/*CBullet* bullet = dynamic_cast<CBullet*>(e->obj);

				if (bullet->isEnemy)
				{
					Hurt();
				}*/
			}

			// Portal
			else if (dynamic_cast<CPortal*>(e->obj))
			{
				CPortal* p = dynamic_cast<CPortal*>(e->obj);
			}
		}
		// Clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}

void CYumetaro::Render()
{
	int ani = -1;

	int alpha = 255;
	if (untouchable) alpha = 128;

	if (nx > 0)
		xReverse = false;
	else if (nx < 0)
		xReverse = true;

	switch (state)
	{
	case YUMETARO_STATE_IDLE:
		ani = YUMETARO_ANI_IDLE;
		break;
	case YUMETARO_STATE_WALK:
		ani = YUMETARO_ANI_WALK;
		break;
	case YUMETARO_STATE_JUMP:
		ani = YUMETARO_ANI_JUMP;
		break;
	default:
		break;
	}

	animation_set->at(ani)->Render(x, y, xReverse, yReverse, false, alpha);

	RenderBoundingBox();
}

void CYumetaro::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case YUMETARO_STATE_IDLE:
		vx = 0;
		break;
	case YUMETARO_STATE_DIE:
		vx = 0;
		vy = -YUMETARO_DIE_DEFLECT_SPEED;
		die_start = GetTickCount();
		break;
	}
}

void CYumetaro::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + YUMETARO_BBOX_WIDTH;
	bottom = top + YUMETARO_BBOX_HEIGHT;

	CGameObject::GetBoundingBox(left, top, right, bottom);
}

void CYumetaro::SetBoundingBox()
{
	left = x;
	top = y;
	right = left + YUMETARO_BBOX_WIDTH;
	bottom = top + YUMETARO_BBOX_HEIGHT;
}

void CYumetaro::AddPointFactor()
{
	if (canMultiScoreJump || canMultiScoreLand)
		pointFactor++;
}

/*
	Reset Mario status to the beginning state of a scene
*/
void CYumetaro::Reset()
{
	SetState(YUMETARO_STATE_IDLE);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
	canJump = true;
	canJumpHigher = true;
}

void CYumetaro::Hurt()
{
	//if (untouchable == 0)
	//{
	//	if (isOnTitleScene)
	//	{
	//		// Transform, stop other objs
	//		isTransform = true;
	//		SetState(YUMETARO_STATE_IDLE);
	//		level = YUMETARO_LEVEL_SMALL;
	//		y += YUMETARO_BIG_BBOX_HEIGHT - YUMETARO_SMALL_BBOX_HEIGHT;
	//		transform_start = GetTickCount();
	//		CGame::GetInstance()->GetCurrentScene()->SetObjStop(true);
	//		StartUntouchable();
	//	}
	//	else
	//	{
	//		if (level > YUMETARO_LEVEL_BIG)
	//		{
	//			// Transform, stop other objs
	//			isTransform = true;
	//			SetState(YUMETARO_STATE_IDLE);
	//			level = YUMETARO_LEVEL_BIG;
	//			transform_start = GetTickCount();
	//			CGame::GetInstance()->GetCurrentScene()->SetObjStop(true);
	//			StartUntouchable();
	//		}
	//		else if (level > YUMETARO_LEVEL_SMALL)
	//		{
	//			// Transform, stop other objs
	//			isTransform = true;
	//			SetState(YUMETARO_STATE_IDLE);
	//			level = YUMETARO_LEVEL_SMALL;
	//			y += YUMETARO_BIG_BBOX_HEIGHT - YUMETARO_SMALL_BBOX_HEIGHT;
	//			transform_start = GetTickCount();
	//			CGame::GetInstance()->GetCurrentScene()->SetObjStop(true);
	//			StartUntouchable();
	//		}
	//		else
	//			SetState(YUMETARO_STATE_DIE);
	//	}
	//}
}

void CYumetaro::OnIntersect(CGameObject* obj, vector<LPGAMEOBJECT>* coObjs)
{
	if (obj != NULL && !obj->isDie && canHit)
	{
		// Bullet Enemy
		if (dynamic_cast<CBullet*>(obj))
		{
			CBullet* bullet = dynamic_cast<CBullet*>(obj);
			if (bullet->isEnemy)
			{
				Hurt();
			}
		}
	}
}
