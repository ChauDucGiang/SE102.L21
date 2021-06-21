#include "Carousel.h"


void CCarousel::Render()
{
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}
CCarousel::CCarousel(float r, float b,int type) {
	mRight = r;
	mBottom = b;
	SetState(type);
	SetBoundingBox();
}
void CCarousel::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = mRight;
	b = mBottom;

	CGameObject::GetBoundingBox(l, t, r, b);
}

void CCarousel::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case CAROUSEL_STATE_UP:
		carousel_speed = CAROUSEL_SPEED;
		break;
	case CAROUSEL_STATE_BACK:
		carousel_speed = -CAROUSEL_SPEED;
		break;
	}
}
void CCarousel::SetBoundingBox()
{
	left = x;
	top = y;
	right = mRight;
	bottom = mBottom;
}