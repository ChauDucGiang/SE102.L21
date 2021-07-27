#include "SlopeBrick.h"
#include "Yumetaro.h"
#include "BlackMonster.h"
#include <dinput.h>
#include "Game.h"
#include "BallGun.h"



void CSlopeBrick::Render()
{
	//animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}
CSlopeBrick::CSlopeBrick(float r, float b, int type) {
	mRight = r;
	mBottom = b;
	//ratio_hw = 0.5;
	//ratio_hw = (y-b) / (r - x);
	SetState(type);
	SetBoundingBox();
}
void CSlopeBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = mRight;
	b = mBottom;
	CGameObject::GetBoundingBox(l, t, r, b);
}

void CSlopeBrick::Collision(LPGAMEOBJECT object, float dy, float dx)
{
	float l, t, r, b;
	this->GetBoundingBox(l, t, r, b);

	float obj_l, obj_t, obj_r, obj_b;
	object->GetBoundingBox(obj_l, obj_t, obj_r, obj_b);

	//if (!this->isContain(object))
		//return;
	// Bullet
	if (dynamic_cast<CBullet*>(object))
	{
		float obj_width = obj_r - obj_l;
		float obj_height /*= obj_t - obj_b - 2*/;

		if (state == SLOPE_STATE_UP)
		{
			if (object->vx <= 0)	// Go down
			{
				obj_height = obj_t - obj_b - 2;
			}
			else					// Go up
			{
				obj_height = obj_t - obj_b;
			}

			if (obj_r + dx  < l || obj_r - 4 + dx > r || obj_b + dy > t)
			{
				object->isColSlopeBrick = false;
				return;
			}

			float check_x = obj_r + dx;
			float check_y = obj_b + dy;

			float y_col = this->y - (r - check_x) * this->ratio_hw;

			if (check_y < y_col)
			{
				object->isColSlopeBrick = true;
				object->vy = 0;
				check_x = check_x - obj_width;
				check_y = y_col + obj_height;

				object->SetPosition(check_x - dx, check_y - dy);
				if (object->vx >= 0)	// Go up
				{
					object->vx -= 0.005f;
				}
				else					// Go down
				{
					if (object->vx >= -0.1f) object->vx += -0.002f;
				}
			}
		}
		else if (state == SLOPE_STATE_DOWN) {
			if (object->vx >= 0)	// Go down
			{
				obj_height = obj_t - obj_b - 2;
			}
			else					// Go up
			{
				obj_height = obj_t - obj_b;
			}

			if (obj_l + 4 + dx  < l || obj_l + dx > r || obj_b + dy > t)
			{
				object->isColSlopeBrick = false;
				return;
			}

			float check_x = obj_l + dx;
			float check_y = obj_b + dy;

			float y_col = this->y - (check_x - l) * this->ratio_hw;

			if (check_y < y_col)
			{
				object->isColSlopeBrick = true;
				object->vy = 0;
				check_y = y_col + obj_height;

				object->SetPosition(check_x - dx, check_y - dy);
				if (object->vx <= 0)	// Go up
				{
					object->vx -= 0.005f;
				}
				else					// Go down
				{
					if (object->vx <= 0.1f) object->vx += 0.002f;
				}
			}
		}
	}
	
	// Time bomb
	else if (dynamic_cast<CTimeBomb*>(object)) {
		float obj_width = obj_r - obj_l;
		float obj_height = obj_t - obj_b + 6;

		if (state == SLOPE_STATE_UP)
		{

			//if (obj_r + dx  < l || obj_r - 4 + dx > r || obj_b + dy < t)
			if (obj_r + dx  < l || obj_r - 4 + dx > r || obj_b + dy > t)
			{
				object->isColSlopeBrick = false;
				return;
			}

			float check_x = obj_r + dx;
			float check_y = obj_b + dy;

			float y_col = this->y - (r - check_x) * this->ratio_hw;

			if (check_y < y_col)
			{
				object->isColSlopeBrick = true;
				object->vy = 0;
				check_x = check_x - obj_width;
				check_y = y_col + obj_height;

				object->SetPosition(check_x - dx, check_y - dy);
				if (object->state == TIME_BOMB_STATE_FALL) {
					object->SetState(TIME_BOMB_STATE_EXPLODE);
				}

				/*if (dynamic_cast<CTimeBomb*>(object)) {
					CTimeBomb* timeBomb = dynamic_cast<CTimeBomb*>(object);
					object->SetState(TIME_BOMB_STATE_EXPLODE);
				}*/
			}
		}
		else if (state == SLOPE_STATE_DOWN) {
			//if (obj_l + 4 + dx  < l || obj_l + dx > r || obj_b + dy < t)
			if (obj_l + 4 + dx  < l || obj_l + dx > r || obj_b + dy > t)
			{
				object->isColSlopeBrick = false;
				return;
			}

			float check_x = obj_l + dx;
			float check_y = obj_b + dy;

			float y_col = this->y - (check_x - l) * this->ratio_hw;

			if (check_y < y_col)
			{
				object->isColSlopeBrick = true;
				object->vy = 0;
				check_y = y_col + obj_height;
				object->SetPosition(check_x - dx, check_y - dy);
				if (object->state == TIME_BOMB_STATE_FALL) {
					object->SetState(TIME_BOMB_STATE_EXPLODE);
				}
				/*	if (dynamic_cast<CTimeBomb*>(object)) {
						CTimeBomb* timeBomb = dynamic_cast<CTimeBomb*>(object);
						timeBomb->SetState(TIME_BOMB_STATE_EXPLODE);
					}*/
			}
		}
	}
	// Yumetaro
	else if (dynamic_cast<CYumetaro*>(object) )
	{
		CYumetaro* yumetaro = dynamic_cast<CYumetaro*>(object);
		float obj_width = obj_r - obj_l;
		float obj_height = obj_t - obj_b;
		if (state == SLOPE_STATE_UP)
		{
			if (obj_r + dx  < l || obj_r - 4 + dx > r || obj_b + dy > t) {
				object->isColSlopeBrick = false;
				return;
			}
			float check_x = obj_r + dx;
			float check_y = obj_b + dy;

			float y_col = this->y - (r - check_x) * this->ratio_hw;

			if (check_y < y_col)
			{
				yumetaro->SetState(YUMETARO_STATE_WALK);
				object->isColSlopeBrick = true;
				object->vy = 0;
				check_x = check_x - obj_width;
				check_y = y_col + obj_height;

				object->SetPosition(check_x - dx, check_y - dy);
				if (object->vx < 0)	// Go down
				{
					if (object->nx < 0)
					{
						if (object->vx >= -0.12f)
							object->vx -= 0.008f;
					}
					else if (object->nx > 0)
					{
						if (object->vx >= -0.12f)
							object->vx -= 0.007f;
					}
					if (CGame::GetInstance()->IsKeyDown(DIK_RIGHT))
					{
						if (object->vx <= 0.06f)
							object->vx += 0.005f;
					}
				}
				else if (yumetaro->vx >= 0)
				{
					if (!CGame::GetInstance()->IsKeyDown(DIK_RIGHT))
					{
						if (object->vx >= -0.12f)
							object->vx -= 0.0045f;
					}
				}

				// Logic with jump when intersert SlopeBrick
				if (yumetaro->canRepeatJump)
				{
					yumetaro->isOnGround = true;
					yumetaro->canJump = true;
					yumetaro->canJumpHigher = true;
				}
			}
		}
		else if (state == SLOPE_STATE_DOWN) {
			if (obj_l + 4 + dx  < l || obj_l + dx > r || obj_b + dy > t) {
				object->isColSlopeBrick = false;
				return;
			}
			float check_x = obj_l + dx;
			float check_y = obj_b + dy;

			float y_col = this->y - (check_x - l) * this->ratio_hw;

			if (check_y < y_col)
			{
				yumetaro->SetState(YUMETARO_STATE_WALK);
				object->isColSlopeBrick = true;
				object->vy = 0;
				check_y = y_col + obj_height;
				object->SetPosition(check_x - dx, check_y - dy);
				if (object->vx > 0 )	// Go down
				{
					if (object->nx > 0)
					{
						if (object->vx <= 0.12f)
							object->vx += 0.008f;
					}
					else if (object->nx < 0)
					{
						if (object->vx <= 0.12f)
							object->vx += 0.007f;
					}
					if (CGame::GetInstance()->IsKeyDown(DIK_LEFT))
					{
						if (object->vx >= -0.06f)
							object->vx -= 0.005f;
					}
				}
				else if (yumetaro->vx <= 0)
				{
					if (!CGame::GetInstance()->IsKeyDown(DIK_LEFT)) 
					{
						if (object->vx <= 0.12f)
							object->vx += 0.0045f;
					}				
				}
				//else					// Go up
				//{
				//	object->vx -= 0.007f;
				//}

				// Logic with jump when intersert SlopeBrick
				if (yumetaro->canRepeatJump)
				{
					yumetaro->isOnGround = true;
					yumetaro->canJump = true;
					yumetaro->canJumpHigher = true;
				}
			}
		}
	}
	// Black monters
	else if (dynamic_cast<CBlackMonster*>(object))
	{
		CBlackMonster* blackMonster = dynamic_cast<CBlackMonster*>(object);
		float obj_width = obj_r - obj_l;
		float obj_height;

		if (state == SLOPE_STATE_UP)
		{
			if (object->vx <= 0)	// Go down
			{
				obj_height = obj_t - obj_b - 2;
			}
			else					// Go up
			{
				obj_height = obj_t - obj_b;
			}

		/*	if (obj_r + dx  < l || obj_r - 4 + dx > r || obj_b + dy < t)*/
			if (obj_r + dx  < l || obj_r - 4 + dx > r || obj_b + dy > t)
			{
				object->isColSlopeBrick = false;
				blackMonster->vx = blackMonster->nx * BLACK_MONSTER_WALKING_SPEED;
				return;
			}

			float check_x = obj_r + dx;
			float check_y = obj_b + dy;

			float y_col = this->y - (r - check_x) * this->ratio_hw;

			object->isColSlopeBrick = true;
			object->vy = 0;
			check_x = check_x - obj_width;
			check_y = y_col + obj_height;

			object->SetPosition(check_x - dx, check_y - dy);
			if (object->vx >= 0)	// Go up
			{
				object->vx = blackMonster->nx * BLACK_MONSTER_WALKING_SPEED / 1.5;
			}
			else					// Go down
			{
				object->vx = blackMonster->nx * 1.5 * BLACK_MONSTER_WALKING_SPEED;
			}
			//else blackMonster->vx = blackMonster->nx * BLACK_MONSTER_WALKING_SPEED;
		}
		else if (state == SLOPE_STATE_DOWN) 
		{
			if (object->vx >= 0)	// Go down
			{
				obj_height = obj_t - obj_b - 2;
			}
			else					// Go up
			{
				obj_height = obj_t - obj_b;
			}

			//if (obj_l + 4 + dx  < l || obj_l + dx > r || obj_b + dy < t)
			if (obj_l + 4 + dx  < l || obj_l + dx > r || obj_b + dy > t)
			{
				object->isColSlopeBrick = false;
				blackMonster->vx = blackMonster->nx * BLACK_MONSTER_WALKING_SPEED;
				return;
			}

			float check_x = obj_l + dx;
			float check_y = obj_b + dy;

			float y_col = this->y - (check_x - l) * this->ratio_hw;

			object->isColSlopeBrick = true;
			object->vy = 0;
			check_y = y_col + obj_height;

			object->SetPosition(check_x - dx, check_y - dy);
			if (object->vx <= 0)	// Go up
			{
				object->vx = blackMonster->nx * BLACK_MONSTER_WALKING_SPEED / 1.5;
			}
			else					// Go down
			{
				object->vx = blackMonster->nx * 1.5 * BLACK_MONSTER_WALKING_SPEED;
			}
			//else blackMonster->vx = blackMonster->nx * BLACK_MONSTER_WALKING_SPEED;
		}
	}

}

void CSlopeBrick::SetState(int state)
{
	CGameObject::SetState(state);

}
void CSlopeBrick::SetBoundingBox()
{
	left = x;
	top = y;
	right = mRight;
	bottom = mBottom;
}