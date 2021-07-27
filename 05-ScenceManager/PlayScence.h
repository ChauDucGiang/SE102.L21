#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "Ground.h"
#include "TileMap.h"
#include "Hud.h"
#include "Grid.h"
#include "Yumetaro.h"
#include "BlackMonster.h"
#include "Carousel.h"
#include "SlopeBrick.h"
#include  "PlatformsMoving.h"
#include "TreasureTree.h"
#include "Water.h"
#include "Worm.h"
#include "BallGun.h"
#include "TimeBomb.h"
#include "Elevator.h"
#include "Window.h"
#include "Treasure.h"
#include "Boss.h"
#include "Tube.h"
#include "BlackPipe.h"
#include "Zone.h"
#include "Quadtree.h"

class CPlayScene: public CScene
{
protected: 
	CYumetaro *player = NULL;					// A play scene has to have player, right?
	CTileMap* map = NULL;
	CGrid* grid = NULL;
	CHud* hud = NULL;

	vector<LPGAMEOBJECT> viewOtherObjs;
	vector<LPGAMEOBJECT> viewObjs;
	vector<LPGAMEOBJECT> viewAfterObjs;

	Quadtree* quadtree;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAP(string line);
	void _ParseSection_GRID(string line);
	
public: 
	bool isBrickToCoin = false;

	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	void EndScene();
	void ChangeYumetaroLocation(int childSceneId, bool isCameraStatic, float x, float y);
	void ChangeChildScene(int childSceneId);

	// Cập nhật vị trí camera khi đụng biên map và khi Mario di chuyển // Chưa dùng đến
	//void UpdateCamera(int mapWidth, int mapHeight);

	CYumetaro* GetPlayer() { return player; }
	CTileMap* GetMap() { return map; }
	CGrid* GetGrid() { return grid; }
	void PushBackObj(CGameObject* obj);

	//friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public: 
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CPlayScenceKeyHandler(CScene *s) :CScenceKeyHandler(s) {};
};

