#include <d3dx9.h>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"

/*
	Initialize game object and load a texture
*/
CGameObject::CGameObject(float x, float y, LPDIRECT3DTEXTURE9 tex)
{
	this->x = x;
	this->y = y;
	this->texture = tex;
}

void CGameObject::Render()
{
	CGame::GetInstance()->Draw(x, y, texture);
}

CGameObject::~CGameObject()
{
	if (texture != NULL) texture->Release();
}

#define YUMETARO_VX 0.1f
#define YUMETARO_WIDTH 14

void CMario::Update(DWORD dt)
{
	x += vx*dt;

	int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();
	if (x <= 0 || x >= BackBufferWidth - YUMETARO_WIDTH) {
		
		vx = -vx;

		if (x <= 0)
		{
			x = 0;
		}
		else if (x >= BackBufferWidth - YUMETARO_WIDTH)
		{
			x = (float)(BackBufferWidth - YUMETARO_WIDTH);
		}
	}
}
