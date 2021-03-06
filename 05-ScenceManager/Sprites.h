#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <unordered_map>

using namespace std;

class CSprite
{
	int id;				// Sprite ID in the sprite database

	int left; 
	int top;
	int right;
	int bottom;
	int xD, yD;
	bool isMap;
	LPDIRECT3DTEXTURE9 texture;
public: 
	CSprite(int id, int left, int top, int right, int bottom, int xD, int yD, LPDIRECT3DTEXTURE9 tex,bool isMap=false);

	void Draw(float x, float y, bool xReverse = false, bool yReverse = false, int alpha = 255);
	void DrawSprite(float x, float y, int alpha = 255);
};

typedef CSprite * LPSPRITE;

/*
	Manage sprite database
*/
class CSprites
{
	static CSprites * __instance;

	unordered_map<int, LPSPRITE> sprites;

public:
	void Add(int id, int left, int top, int right, int bottom, int xD, int yD, LPDIRECT3DTEXTURE9 tex);
	LPSPRITE Get(int id);
	void CSprites::Clear();

	static CSprites * GetInstance();
};



