#include "Window.h"
#include "Game.h"

CWindow::CWindow()
{
	
	SetState(WINDOW_STATE_CLOSE);
	SetBoundingBox();
}

CWindow::CWindow(float x, float y)
{
	SetPosition(x, y);
	SetBoundingBox();
}

void CWindow::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	
	CYumetaro* yumetaro = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	float distanceX = x - yumetaro->x;
	
	if (state == WINDOW_STATE_CLOSE && abs(distanceX) <= 50)
		SetState(WINDOW_STATE_OPEN);
	if (state == WINDOW_STATE_OPEN && GetTickCount64() - start_open > WINDOW_TIME_START ) {
		SetState(WINDOW_STATE_ATTACK);
		
	}
	if (monster != NULL) {
		if(!monster->isDead)
		monster->Update(dt, coObjects);
	}
	
	/*if (state == WINDOW_STATE_OPEN) {
		CBlackMonster* monster = new CBlackMonster(x);
		CGame::GetInstance()->GetCurrentScene()->GetBehindObjs()->push_back(monster);
	}*/
	

}

void CWindow::Render()
{
	int ani = WINDOW_ANI_CLOSE;

	switch (state) {
	case WINDOW_STATE_CLOSE:
		ani = WINDOW_ANI_CLOSE;
		break;
	case WINDOW_STATE_OPEN:
		ani = WINDOW_ANI_OPEN;
		break;
	case WINDOW_STATE_ATTACK:
		ani = WINDOW_ANI_ATTACK;
		break;
	default:
		break;
	}
	
	animation_set->at(ani)->Render(x, y);
	if (monster != NULL) {
		if(!monster->isDead)
		monster->Render();
	}
	RenderBoundingBox();
}

void CWindow::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + WINDOW_BBOX_WIDTH;
	b = y - WINDOW_BBOX_HEIGHT;
	CGameObject::GetBoundingBox(l, t, r, b);
}

void CWindow::SetBoundingBox()
{
	left = x;
	top = y;
	right = x + WINDOW_BBOX_WIDTH;
	bottom = y - WINDOW_BBOX_HEIGHT;
}
void CWindow::SetState(int state) {
	CGameObject::SetState(state);
	switch (state) {
	case WINDOW_STATE_CLOSE:
		break;
	case WINDOW_STATE_OPEN:
		start_open = GetTickCount64();
		break;

	case WINDOW_STATE_ATTACK:
		monster = new CBlackMonster(x + WINDOW_BBOX_WIDTH/2 , y);
		((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->PushBackObj(monster);
		break;

	}
}