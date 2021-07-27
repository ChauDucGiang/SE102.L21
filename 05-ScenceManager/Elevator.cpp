#include "Elevator.h"
#include "PlayScence.h"

CElevator::CElevator()
{
	SetState(ELEVATOR_STATE_IDLE);
	SetBoundingBox();
}

void CElevator::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	x += dx;
	y += dy;

	/*MoveThrough(OBJ_MOVE_XY);*/
	if (x > ELEVATOR_X_MAX && state == ELEVATOR_STATE_IDLE) {
		SetState(ELEVATOR_STATE_FALL);
	}
	if (state != ELEVATOR_STATE_FALL)
	{
		for (UINT i = 0; i < coObjects->size(); i++)
		{
			if (AABBCheck(this, coObjects->at(i)))
			{
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
		}
		// Clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
	// Get others instance
}

void CElevator::Render()
{
	int ani = ELEVATOR_ANI_IDLE;
	switch (state) {
	case ELEVATOR_STATE_IDLE:
		ani = ELEVATOR_ANI_IDLE;
		break;
	case ELEVATOR_STATE_FALL:
		ani = ELEVATOR_ANI_FALL;
		break;
	default:
		break;

	}
	animation_set->at(ani)->Render(x, y, xReverse, yReverse);

	RenderBoundingBox();
}

void CElevator::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state) {
	case ELEVATOR_STATE_IDLE:
		vx = 0;
		break;
	case ELEVATOR_STATE_FALL:
		vx = 0;
		break;
	default:
		break;

	}

}

void CElevator::OnIntersect(CGameObject* obj, vector<LPGAMEOBJECT>* coObjs)
{

}

void CElevator::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y - 63;
	right = x + ELEVATOR_BBOX_WIDTH;
	if (state == ELEVATOR_STATE_IDLE) {
		bottom = y - ELEVATOR_BBOX_IDLE_HEIGHT;
	}
	else if (state == ELEVATOR_STATE_FALL) {
		bottom = y - ELEVATOR_BBOX_FALL_HEIGHT;
	}
	

	CGameObject::GetBoundingBox(left, top, right, bottom);
}

void CElevator::SetBoundingBox()
{
	left = x;
	top = y + 63;
	right = x + ELEVATOR_BBOX_WIDTH;
	if (state == ELEVATOR_STATE_IDLE) {
		bottom = y - ELEVATOR_BBOX_IDLE_HEIGHT;
	}
	else if (state == ELEVATOR_STATE_FALL) {
		bottom = y - ELEVATOR_BBOX_FALL_HEIGHT;
	}
}
