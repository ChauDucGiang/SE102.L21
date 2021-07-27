#include "BlackPipe.h"

CBlackPipe::CBlackPipe(float x, float y, int r, int b, int type, int destinationDown, int destinationUp, float tele_y)
{
	mRight = r;
	mBottom = b;
	this->type = type;
	this->destinationDown = destinationDown;
	this->destinationUp = destinationUp;
	this->tele_y = tele_y;

	SetBoundingBox();
}

void CBlackPipe::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs)
{
}

void CBlackPipe::Render()
{
	RenderBoundingBox();
}

void CBlackPipe::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = mRight;
	b = mBottom;

	CGameObject::GetBoundingBox(l, t, r, b);
}

void CBlackPipe::SetBoundingBox()
{
	left = x;
	top = y;
	right = mRight;
	bottom = mBottom;
}