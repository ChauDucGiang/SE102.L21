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
#include "Carousel.h"

CYumetaro::CYumetaro(float x, float y) : CGameObject()
{
	untouchable = 0;
	SetState(YUMETARO_STATE_IDLE);

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;

	bullets = new vector<CStar*>();
	// Not use
	for (int i = 0; i < YUMETARO_NUM_BULLETS; i++)
	{
		CStar* bullet = new CStar();
		bullet->SetPosition(x, y);
		bullets->push_back(bullet);
		CGame::GetInstance()->GetCurrentScene()->GetFrontObjs()->push_back(bullet);
	}

	SetBoundingBox();
}

void CYumetaro::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	/*DebugOut(L"[Yumetaro InSlopeBrick]: %d\n", isColSlopeBrick);
	
	DebugOut(L"[Yumetaro isExplode]: %d\n", isExplode);
	DebugOut(L"[Yumetaro vy]: %f\n", vy);*/
	//DebugOut(L"[Yumetaro isOnGround]: %d\n", isOnGround);

	// Get others instance
	CCamera* camera = CCamera::GetInstance();
	int leftMap = camera->GetLeftMap();
	int topMap = camera->GetTopMap();
	int rightMap = camera->GetRightMap();
	int bottomMap = camera->GetBottomMap();
	int heightMap = camera->GetHeightMap();
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	int childSceneId = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->childSceneId;

	CGameObject::Update(dt);

	// Simple fall down
	if (!CGame::GetInstance()->GetCurrentScene()->GetIsObjStop() && state != YUMETARO_STATE_PIPE)
	{
		if (isOnTitleScene)
			vy -= YUMETARO_TITLE_SCENE_GRAVITY * dt;
		else
			vy -= YUMETARO_GRAVITY * dt;
	}

	// Max jump
	if (!isOnTitleScene && vy >= YUMETARO_MAX_Y_SPEED)
	{
		vy = YUMETARO_MAX_Y_SPEED;
		canJumpHigher = false;
	}

	// Die
	if (state == YUMETARO_STATE_DIE)
	{
		//MoveThrough(OBJ_MOVE_XY);

		if (GetTickCount64() - die_start > YUMETARO_DIE_TIME)
		{
			Reset();
			/*CPlayerInfo::GetInstance()->AdjustLife(-LIFE_1);
			CGame::GetInstance()->SwitchScene(WORLD_MAP_1);*/
		}
		return;
	}

	// Yumetaro when col edge map
	if (x <= leftMap) {
		//x = leftMap;
		if (scene->childSceneId == MAP_1_8) {
			scene->ChangeChildScene(MAP_1_9);
		}
		if (scene->childSceneId == MAP_1_4) {
			scene->ChangeChildScene(MAP_1_3);
		}
	}
		
	if (y >= topMap)
	{
		//y = topMap;
		if (scene->childSceneId == MAP_1_1)	// Change MAP_1_1 -> MAP_1_0
		{
			scene->ChangeChildScene(MAP_1_0);
		}
		/*else y = topMap;*/
	}
	if (x + YUMETARO_BBOX_WIDTH >= rightMap)
	{
		//x = rightMap - YUMETARO_BBOX_WIDTH;
		if (scene->childSceneId == MAP_1_9) {
			scene->ChangeChildScene(MAP_1_8);
		}
		else if (scene->childSceneId == MAP_1_3) {
			
			scene->ChangeChildScene(MAP_1_4);
		}
		else if (scene->childSceneId == MAP_1_4) {
			
			scene->ChangeChildScene(MAP_1_5);
		}
		
	}
	
	if (y - YUMETARO_BBOX_HEIGHT <= bottomMap)
	{
		if (scene->childSceneId == MAP_1_0)	// Change MAP_1_0 -> MAP_1_1
		{
			scene->ChangeChildScene(MAP_1_1);
		}
		if (scene->childSceneId == MAP_1_8) {
			//SetState(YUMETARO_STATE_DIE);
			//scene->ChangeChildScene(MAP_1_8);
		}
		if (scene->childSceneId == MAP_1_7) {
			scene->ChangeChildScene(MAP_1_6);
		}
	}
	
	HandleAction();
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (AABBCheck(this, coObjects->at(i)))
		{
			OnIntersect(coObjects->at(i), coObjects);
			if (dynamic_cast<CSlopeBrick*>(coObjects->at(i)))
			{
				CSlopeBrick* brick = dynamic_cast<CSlopeBrick*>(coObjects->at(i));
				brick->Collision(this, dy, dx);

			}
			if (dynamic_cast<CPlatformsMoving*>(coObjects->at(i)))
			{
				CPlatformsMoving* pm = dynamic_cast<CPlatformsMoving*>(coObjects->at(i));
				if (pm->GetState() == PM_STATE_HEIGHT/* && pm->vy < 0*/) {
					OnIntersect(coObjects->at(i), coObjects);

				}
			}
		}

		/*if (dynamic_cast<CPlatformsMoving*>(coObjects->at(i))) {
			CPlatformsMoving* brick = dynamic_cast<CPlatformsMoving*>(coObjects->at(i));
			brick->Collision(this, dy, dx, dt);
		}*/
	}


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// Turn off collision when die 
	if (state != YUMETARO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);
	
	// Reset untouchable timer if untouchable time has passed
	if (GetTickCount64() - untouchable_start > YUMETARO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}
	if (GetTickCount64() - time_explode > YUMETARO_EXPLODE_TIME && state == YUMETARO_STATE_STUN) {
		isExplode = false;
		//untouchable_start = 0;
		SetState(YUMETARO_STATE_IDLE);
	}
	
	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		MoveThrough(OBJ_MOVE_XY);

		if (vy != 0)
		{
			if (!(isColSlopeBrick && state != YUMETARO_STATE_JUMP)) {
				isOnGround = false;
				if (vy < 0) {
					//DebugOut(L"NO CAN JUMP\n");
					canJump = false;
				}
			}
		}		
	}
	else
	{
		
		//MoveThrough(OBJ_MOVE_XY);
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
		colX = nx;

		/*x += min_tx * dx + nx * YUMETARO_DEFLECT_SPEED;
		y += min_ty * dy + ny * YUMETARO_DEFLECT_SPEED;*/

		if (nx == 0)
			x += min_tx * dx + nx * YUMETARO_DEFLECT_SPEED;
		if (ny == 0)
			y += min_ty * dy + ny * YUMETARO_DEFLECT_SPEED;

		// Col with objs
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			//DebugOut(L"[Yumetaro ny]: %f\n", ny);

			e->obj->colX = e->nx;
			e->obj->colY = e->ny;
			// Stand on obj
			if (ny > 0 && e->obj != NULL && !dynamic_cast<CSlopeBrick*>(e->obj) && !dynamic_cast<CElevator*>(e->obj)&& !dynamic_cast<CTreasure*>(e->obj) && !dynamic_cast<CTreasureTree*>(e->obj) && !dynamic_cast<CPlatformsMoving*>(e->obj) && !dynamic_cast<CWater*>(e->obj))
			{
				// Stand-Y
				if (state != YUMETARO_STATE_DIE)
					PreventMoveY(e->obj);
			}

			// Logic with jump
			if (ny > 0 && vy <= 0)
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
			else if (ny < 0 && !dynamic_cast<CBullet*>(e->obj) && !dynamic_cast<CBlackPipe*>(e->obj))
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
			if (dynamic_cast<CBlackMonster*>(e->obj)) {
				MoveThrough(OBJ_MOVE_XY);
			}
			if (dynamic_cast<CSlopeBrick*>(e->obj))
			{
				/*vy = 0;*/
				isColSlopeBrick = true;
				//MoveThrough(OBJ_MOVE_XY);
				if (e->nx != 0)
					x += dx;
				else if (e->ny != 0)
					y += dy;
			}
			else if (dynamic_cast<CPlatformsMoving*>(e->obj)) {
				CPlatformsMoving* pm = dynamic_cast<CPlatformsMoving*>(e->obj);

				if (e->ny > 0)
				{
					if (pm->GetState() == PM_STATE_WIDTH)
					{
						x += pm->dx;
						vy = -0.0015;
						y = pm->y + YUMETARO_BBOX_HEIGHT;
						//pm->Collision(this, this->dy, this->dx, dt);
					}
					else if (pm->GetState() == PM_STATE_HEIGHT)
					{
						y = pm->y + YUMETARO_BBOX_HEIGHT;
					}
				}
			}
			else if (dynamic_cast<CCarousel*>(e->obj))
			{
				if (e->ny > 0) {
					CCarousel* carousel = dynamic_cast<CCarousel*>(e->obj);
					if (carousel->GetState() != NULL) {
						x += carousel->GetBrickSpeed() * dt;
					}
				}

			}
			// Treasure
			else if (dynamic_cast<CTreasureTree*>(e->obj))
			{
				/*x += dx;
				y += dy;*/
				CTreasureTree* treasureTree = dynamic_cast<CTreasureTree*>(e->obj);
				if (!treasureTree->isDie)
				{
					EatTreasure(treasureTree, coObjects);
				}
			}
			//
			else if (dynamic_cast<CTreasure*>(e->obj))
			{
				/*x += dx;
				y += dy;*/
				CTreasure* medicine = dynamic_cast<CTreasure*>(e->obj);
				if (!medicine->isDie)
				{
					EatMedicine(medicine, coObjects);
				}
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
			//Water
			else if (dynamic_cast<CWater*>(e->obj)) {
				MoveThrough(OBJ_MOVE_XY);
			}
			else if (dynamic_cast<CTube*>(e->obj)) {
				MoveThrough(OBJ_MOVE_XY);
				
			}
			//
			else if (dynamic_cast<CTimeBomb*>(e->obj)|| dynamic_cast<CWorm*>(e->obj)) {
				MoveThrough(OBJ_MOVE_XY);
			}
			// Portal
			else if (dynamic_cast<CPortal*>(e->obj))
			{
				CPortal* p = dynamic_cast<CPortal*>(e->obj);
			}
			// Black pipe
			else if (dynamic_cast<CBlackPipe*>(e->obj))
			{				
				if (state == YUMETARO_STATE_PIPE)
				{
					MoveThrough(OBJ_MOVE_Y);
				}

				CBlackPipe* pipe = dynamic_cast<CBlackPipe*>(e->obj);
				if (left >= pipe->left && right <= pipe->right + 1)
				{					
					switch (pipe->type)
					{
					case BLACK_PIPE_TYPE_DOWN:
						if (e->ny > 0 && CGame::GetInstance()->IsKeyDown(DIK_DOWN))
						{
							x = pipe->left + 0.8f;
							pipe_tele_y = pipe->tele_y;
							pipe_top = pipe->top;
							pipe_bottom = pipe->bottom;							
							destinationDown = pipe->destinationDown;							

							y -= YUMETARO_PIPE_DOWN_HEIGHT;
							SetState(YUMETARO_STATE_PIPE);							
							pipe_down_start = GetTickCount64();							
						}
						else if (e->ny < 0)
						{
							x = pipe->left + 0.5f;
							pipe_tele_y = pipe->tele_y;
							pipe_top = pipe->top;
							pipe_bottom = pipe->bottom;
							destinationUp = pipe->destinationUp;

							SetState(YUMETARO_STATE_PIPE);
							pipe_up_start = GetTickCount64();
						}
						break;
					case BLACK_PIPE_TYPE_UP:						
						break;
					default:
						break;
					}
				}
			}
			// Ball Gun
			else if (dynamic_cast <CBallGun*>(e->obj)) {
				CBallGun* ballGun = dynamic_cast<CBallGun*>(e->obj);
				if (nx < 0) {
					ballGun->vx = YUMETARO_WALKING_SPEED / 2;
				}
				else if (nx > 0) {
					ballGun->vx = -YUMETARO_WALKING_SPEED / 2;
				}

			}
			else if (dynamic_cast<CElevator*>(e->obj))
			{
				CElevator* elevator = dynamic_cast<CElevator*>(e->obj);
				if (elevator->GetState() == ELEVATOR_STATE_IDLE && ny > 0) {
					/*isOnGround = true;
					canJump = true;
					canJumpHigher = true;*/
					x += elevator->dx;
					vy = 0;
					//PreventMoveY(elevator);
					y = elevator->y - ELEVATOR_BBOX_IDLE_HEIGHT + 23;
					elevator->vx = ELEVATOR_X_SPEED;
				}
				else if (elevator->GetState() == ELEVATOR_STATE_FALL) {
					MoveThrough(OBJ_MOVE_Y);
				}
			}

		}
		// Clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}

void CYumetaro::Render()
{
	if (state == YUMETARO_STATE_DIE)
		return;
	int ani = -1;

	int alpha = 255;
	if (untouchable) alpha = 128;
	if (inTube) alpha = 0;

	if (nx > 0)
		xReverse = false;
	else if (nx < 0)
		xReverse = true;

	switch (state)
	{
	case YUMETARO_STATE_IDLE: case YUMETARO_STATE_PIPE:
		ani = YUMETARO_ANI_IDLE;
		break;
	case YUMETARO_STATE_WALK:
		ani = YUMETARO_ANI_WALK;
		break;
	case YUMETARO_STATE_JUMP:
		ani = YUMETARO_ANI_JUMP;
		break;
	case YUMETARO_STATE_STUN:
		ani = YUMETARO_ANI_STUN;
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
	case YUMETARO_STATE_PIPE:
		vx = 0;
		if (pipe_down_start != 0)
			vy = -YUMETARO_PIPE_Y_SPEED;
		else if (pipe_up_start != 0)
			vy = YUMETARO_PIPE_Y_SPEED;
		break;
	case YUMETARO_STATE_DIE:
		CreateDieEffect();
		vx = 0;
		vy = -YUMETARO_DIE_DEFLECT_SPEED;
		die_start = GetTickCount64();
		break;
	case YUMETARO_STATE_STUN:
		isExplode = true;
		time_explode = GetTickCount64();
		break;

	
	}
}

void CYumetaro::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + YUMETARO_BBOX_WIDTH;
	bottom = top - YUMETARO_BBOX_HEIGHT;	
	CGameObject::GetBoundingBox(left, top, right, bottom);
}
void CYumetaro::EatTreasure(CTreasureTree* treasureTree, vector<LPGAMEOBJECT>* coObjs)
{
	CPlayerInfo::GetInstance()->AdjustScore(POINT_1000);
	CPlayerInfo::GetInstance()->AdjustMoney(MONEY_1);
	treasureTree->Dead();
	treasureTree->DeleteObjs(coObjs);
}
void CYumetaro::EatMedicine(CTreasure* medicine, vector<LPGAMEOBJECT>* coObjs)
{
	CPlayerInfo::GetInstance()->AdjustScore(POINT_100);
	CPlayerInfo::GetInstance()->AdjustMoney(MONEY_1);
	medicine->Dead();
	medicine->DeleteObjs(coObjs);
}
void CYumetaro::SetBoundingBox()
{
	left = x;
	top = y;
	right = left + YUMETARO_BBOX_WIDTH;
	bottom = top - YUMETARO_BBOX_HEIGHT;
}

void CYumetaro::AddPointFactor()
{
	if (canMultiScoreJump || canMultiScoreLand)
		pointFactor++;
}

void CYumetaro::HandleAction()
{
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();

	//// Pipe down
	//// Piping down
	if ((GetTickCount64() - pipe_down_start) < 100)
	{
		SetState(YUMETARO_STATE_PIPE);
	}
	else 
	{
		pipe_down_start = 0;
	}

	if ((GetTickCount64() - pipe_up_start) < 100)
	{
		SetState(YUMETARO_STATE_PIPE);
	}
	else
	{
		pipe_up_start = 0;
	}

	if (state == YUMETARO_STATE_PIPE)
	{
		if (vy < 0 && y <= pipe_tele_y)
			scene->ChangeChildScene(destinationDown);
		else if (vy >= 0 && y >= pipe_tele_y)
			scene->ChangeChildScene(destinationUp);
		if (vy <= 0 && y <= pipe_bottom)
			SetState(YUMETARO_STATE_JUMP);
		if (vy >= 0 && bottom >= pipe_top)
			SetState(YUMETARO_STATE_IDLE);
	}
}

/*
	Reset Mario status to the beginning state of a scene
*/
void CYumetaro::Reset()
{
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	int childSceneId = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->childSceneId;
	SetState(YUMETARO_STATE_IDLE);
	switch (scene->childSceneId) {

	case MAP_1_2:
		SetPosition(977, 440);
		break;
	case MAP_1_3:
		scene->ChangeChildScene(MAP_1_2);
		SetPosition(977, 440);
	case MAP_1_8:
		SetPosition(1678, 129);

	default:
		SetPosition(start_x, start_y);
		break;
	}
	
	SetSpeed(0, 0);
	canJump = true;
	canJumpHigher = true;
}

void CYumetaro::Hurt()
{
	if (untouchable == 0)
	{
		if (!isOnTitleScene)
		{
			// Transform, stop other objs
			isTransform = true;
			SetState(YUMETARO_STATE_STUN);
			transform_start = GetTickCount64();
			//CGame::GetInstance()->GetCurrentScene()->SetObjStop(true);
			StartUntouchable();
		}
	}
}

void CYumetaro::OnIntersect(CGameObject* obj, vector<LPGAMEOBJECT>* coObjs)
{
	if (obj != NULL && !obj->isDie /*&& canHit*/)
	{


		// Bullet Enemy
		if (dynamic_cast<CBullet*>(obj))
		{
			SetState(YUMETARO_STATE_IDLE);
			StartUntouchable();
			
		}
		if (dynamic_cast<CBlackMonster*>(obj)) {
			SetState(YUMETARO_STATE_IDLE);
			StartUntouchable();
		}
		if (dynamic_cast<CTimeBomb*>(obj)) {
			SetState(YUMETARO_STATE_IDLE);
			StartUntouchable();

		}
		if (dynamic_cast<CWorm*>(obj)) {
			SetState(YUMETARO_STATE_IDLE);
			StartUntouchable();
		}
		if (dynamic_cast<CWater*>(obj)) {
			SetState(YUMETARO_STATE_DIE);
		}
		if (dynamic_cast<CTube*>(obj)) {
			CTube* tube = dynamic_cast<CTube*>(obj);
		
		}
		if (dynamic_cast<CPlatformsMoving*>(obj))
		{
			CPlatformsMoving* pm = dynamic_cast<CPlatformsMoving*>(obj);
			if (pm->GetState() == PM_STATE_HEIGHT) {
				if (bottom > pm->top)
				{
					y = pm->y + YUMETARO_BBOX_HEIGHT;
				}
			}

		}
	}
}

void CYumetaro::CreateDieEffect() {
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(YUMETARODIEEFFECT_ANI_SET);
	for (int i = 0; i < 16; i++) {
		CYumetaroDieEffect* effect = new CYumetaroDieEffect(i);
		effect->SetPosition(x, y);
		effect->SetAnimationSet(ani_set);
		((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->PushBackObj(effect);
	}
}
