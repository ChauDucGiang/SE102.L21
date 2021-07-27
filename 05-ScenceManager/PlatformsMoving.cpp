#include "PlatformsMoving.h"
#include "Utils.h"
#include "Yumetaro.h"

void CPlatformsMoving::Collision(LPGAMEOBJECT object, float& y)
{

	float l, t, r, b;
	this->GetBoundingBox(l, t, r, b);

	float obj_l, obj_t, obj_r, obj_b;
	object->GetBoundingBox(obj_l, obj_t, obj_r, obj_b);

	float obj_width = obj_r - obj_l;
	float obj_height = obj_t - obj_b;
	if (y - obj_height < t)
	{
		if (dy > 0)
			y = this->y + obj_height + 3;
		else
			y = this->y + obj_height + 1;
	}
}

void CPlatformsMoving::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	x += dx;
	y += dy;
	
	if (state == PM_STATE_WIDTH)
	{
		if (x < start)
		{
			x = start;
			vx = PM_SPEED;
		}
		else if (x > end)
		{
			x = end;
			vx = -PM_SPEED;
		}
		else
		{
			if (vx > 0)
			{
				if (x < (start + 5 + float((end - start) / 2)))
					vx += PM_SPEED_ACCELERATION;
				else vx -= PM_SPEED_ACCELERATION;
			}
			else if (vx < 0)
			{
				if (x > (start - 5 + float((end - start) / 2)))
					vx -= PM_SPEED_ACCELERATION;
				else vx += PM_SPEED_ACCELERATION;
			}
		}
	}

	else if (state == PM_STATE_HEIGHT)
	{
		if (y > start)
		{
			y = start;
			vy = -PM_SPEED;
		}
		else if (y < end)
		{
			y = end;
			vy = PM_SPEED;
		}
		else
		{
			if (vy > 0)
			{
				if (y < (end + 5 + float((start - end) / 2)))
					vy += PM_SPEED_ACCELERATION;
				else vy -= PM_SPEED_ACCELERATION;
			}
			else if (vy < 0)
			{
				if (y > (end - 5 + float((start - end) / 2)))
					vy -= PM_SPEED_ACCELERATION;
				else vy += PM_SPEED_ACCELERATION;
			}
		}
	}

}

void CPlatformsMoving::Render()
{
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}
CPlatformsMoving::CPlatformsMoving(float r, float b,int type) {
	mRight = r;
	mBottom = b;
	SetState(type);
	SetBoundingBox();
}
void CPlatformsMoving::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x+ PM_BBOX_WIDTH;
	b = y- PM_BBOX_HEIGHT;
}

void CPlatformsMoving::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case PM_STATE_WIDTH:
		vx = PM_SPEED;
		vy = 0;
		break;
	case PM_STATE_HEIGHT:
		vy = PM_SPEED;
		vx = 0;
		break;
	}
}
void CPlatformsMoving::SetBoundingBox()
{
	left = x;
	top = y;
	right = x+ PM_BBOX_WIDTH;
	bottom = y- PM_BBOX_HEIGHT;
}