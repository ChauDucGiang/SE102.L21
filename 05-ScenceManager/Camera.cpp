#include "Camera.h"
#include "PlayScence.h"

CCamera* CCamera::__instance = NULL;

CCamera::CCamera()
{
	position = D3DXVECTOR3(0, 0, 0);

	width = CGame::GetInstance()->GetScreenWidth();
	height = CGame::GetInstance()->GetScreenHeight();
}

CCamera* CCamera::GetInstance()
{
	if (__instance == NULL) __instance = new CCamera();
	return __instance;
}

void CCamera::SetPosition(D3DXVECTOR3 pos)
{
	position = pos;
}

void CCamera::SetPosition(float x, float y)
{
	SetPosition(D3DXVECTOR3(x, y, 0));
}

void CCamera::SetMapSize(int left, int top, int right, int bottom, int width, int height)
{
	leftMap = left;
	topMap = top;
	rightMap = right;
	bottomMap = bottom;
	widthMap = width;
	heightMap = height;
}

void CCamera::Update(DWORD dt)
{
	/*DebugOut(L"[GetBound TOP]: %df\n", GetBound().top);
	DebugOut(L"[GetBound Bottom]: %d\n", GetBound().bottom);
	DebugOut(L"[GetBound Left]: %df\n", GetBound().left);
	DebugOut(L"[GetBound Right]: %d\n", GetBound().right);
	DebugOut(L"[Bottom Map]: %d\n", bottomMap);*/
	float cx, cy;
	CYumetaro* yumetaro = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	yumetaro->GetPosition(cx, cy);
	SetPosition(cx, cy);

	if (GetBound().left < leftMap)
	{
		//vi position cua Camera::GetInstance() ma chinh giua Camera::GetInstance()
		//luc nay o vi tri goc ben trai cua the gioi thuc
		SetPosition(leftMap + GetWidth() / 2, GetPosition().y);
	}

	if (GetBound().right > rightMap)
	{
		//luc nay cham goc ben phai cua the gioi thuc
		SetPosition(rightMap - GetWidth() / 2, GetPosition().y);
		//isMoving = false;
	}

	if (GetBound().top > topMap)
	{
		//luc nay cham goc tren the gioi thuc
		SetPosition(GetPosition().x, topMap - GetHeight() / 2);
	}

	if (GetBound().bottom < bottomMap)
	/*if (GetBound().bottom > mapHeight / 2)*/
	{
		//luc nay cham day cua the gioi thuc
		SetPosition(GetPosition().x, -48 + topMap - heightMap + GetHeight() / 2);
	}
	if (!isStatic)
	{
		if (GetBound().bottom < bottomMap)
		{
			SetPosition(GetPosition().x, topMap - bottomMap + GetHeight() / 2);
			isStatic = true;
		}
	}
	else if (GetBound().bottom < bottomMap)
	{

		isStatic = true;
		//luc nay cham day cua the gioi thuc
		SetPosition(GetPosition().x, bottomMap - 42 + GetHeight() / 2);
	}

}

RECT CCamera::GetBound()
{
	RECT bound;

	bound.left = position.x - width / 2;
	bound.right = bound.left + width;
	bound.top = position.y + height / 2;
	bound.bottom = bound.top - height;

	return bound;
}

void CCamera::SetIsMoving(bool isMoving)
{
	this->isMoving = isMoving;
	float cx, cy;
	CYumetaro* yumetaro = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	yumetaro->GetPosition(cx, cy);
	SetPosition(cx, cy);
}

CCamera::~CCamera()
{

}
bool CCamera::InCamera(CGameObject* object)
{
	float l, t, r, b;
	object->GetBoundingBox(l, t, r, b);
	return !(r < GetBound().left || l > GetBound().right || t < GetBound().bottom || b > GetBound().top);
}