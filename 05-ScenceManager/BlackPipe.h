#pragma once
#include "GameObject.h"

#define BLACK_PIPE_TYPE_DOWN	1
#define BLACK_PIPE_TYPE_UP		2

class CBlackPipe : public CGameObject
{
	int mRight, mBottom;	

	void SetBoundingBox();
public:
	int type;
	int destinationDown;
	int destinationUp;
	float tele_y = -1;

	CBlackPipe(float x, float y, int r, int b, int type, int destinationDown, int destinationUp, float tele_y);

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjs);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

