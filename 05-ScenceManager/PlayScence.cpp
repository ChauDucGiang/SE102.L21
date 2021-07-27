#include <iostream>
#include <fstream>

#include "PlayScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "TileMap.h"
#include "Camera.h"
using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN			-1
#define SCENE_SECTION_TEXTURES			2
#define SCENE_SECTION_SPRITES			3
#define SCENE_SECTION_ANIMATIONS		4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS			6
#define SCENE_SECTION_MAP				7
#define SCENE_SECTION_GRID				8

#define OBJECT_TYPE_YUMETARO			0
#define OBJECT_TYPE_GROUND				5
#define OBJECT_TYPE_CAROUSEL			6
#define OBJECT_TYPE_SLOPE				7
#define OBJECT_TYPE_PLATFORMSMOVING		8
#define OBJECT_TYPE_TREASURE_TREE		9
#define OBJECT_TYPE_WATER				10
#define OBJECT_TYPE_WORM				11
#define OBJECT_TYPE_BALLGUN				12
#define OBJECT_TYPE_TIMEBOMB			13
#define OBJECT_TYPE_ELEVATOR			14
#define OBJECT_TYPE_WINDOW				15
#define OBJECT_TYPE_TREASUE				16
#define OBJECT_TYPE_BOSS				17
#define OBJECT_TYPE_TUBE				18
#define OBJECT_TYPE_ZONE				19
#define OBJECT_TYPE_PORTAL				50
#define OBJECT_TYPE_HUD					51
#define OBJECT_TYPE_BLACKMONSTER		60
#define OBJECT_TYPE_BLACK_PIPE			61

//#define MAX_SCENE_LINE					1024


void CPlayScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());
	int xD = 0;
	int yD = 0;
	if (tokens.size() > 7)
	{
		xD = atoi(tokens[6].c_str());
		yD = atoi(tokens[7].c_str());
	}

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, xD, yD, tex);
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations* animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	if (ani_set_id != -1)
		CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

/*
	Parse a line in section [OBJECTS]
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	int objId = atoi(tokens[1].c_str());
	float x = atof(tokens[2].c_str());
	float y = atof(tokens[3].c_str());

	int ani_set_id = atoi(tokens[4].c_str());

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = LPANIMATION_SET();
	if (ani_set_id != -1)
		ani_set = animation_sets->Get(ani_set_id);

	CGameObject* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_YUMETARO:

		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CYumetaro(x, y);
		player = (CYumetaro*)obj;
		break;
	case OBJECT_TYPE_GROUND:
	{
		float r = atof(tokens[5].c_str());
		float b = atof(tokens[6].c_str());
		obj = new CGround(r, b);

	}

	DebugOut(L"[INFO] Player object created!\n");
	break;
	case OBJECT_TYPE_TREASURE_TREE:
	{

		obj = new CTreasureTree();
	}
	break;
	case OBJECT_TYPE_WORM:
	{
		obj = new CWorm(x);
	}
	break;
	case OBJECT_TYPE_BLACK_PIPE:
	{
		float r = atof(tokens[5].c_str());
		float b = atof(tokens[6].c_str());
		int type = atoi(tokens[7].c_str());
		int destinationDown = atoi(tokens[8].c_str());
		int destinationUp = atoi(tokens[9].c_str());
		float tele_y = atoi(tokens[10].c_str());
		obj = new CBlackPipe(x, y, r, b, type, destinationDown, destinationUp, tele_y);
	}
	break;
	case OBJECT_TYPE_PLATFORMSMOVING:
	{
		if (tokens.size() < 9) return;
		float r = atof(tokens[5].c_str());
		float b = atof(tokens[6].c_str());
		int type = atoi(tokens[9].c_str());
		obj = new CPlatformsMoving(r, b, type);
		CPlatformsMoving* pm = (CPlatformsMoving*)obj;
		int start = atoi(tokens[7].c_str());
		int end = atoi(tokens[8].c_str());
		pm->SetStart(start);
		pm->SetEnd(end);

	}
	break;
	case OBJECT_TYPE_CAROUSEL:
	{
		if (tokens.size() < 7) return;
		float r = atof(tokens[5].c_str());
		float b = atof(tokens[6].c_str());
		int type = atoi(tokens[7].c_str());
		obj = new CCarousel(r, b, type);
	}
	break;
	case OBJECT_TYPE_SLOPE:
	{
		if (tokens.size() < 7) return;
		float r = atof(tokens[5].c_str());
		float b = atof(tokens[6].c_str());
		int type = atoi(tokens[7].c_str());
		obj = new CSlopeBrick(r, b, type);

		float ratio_hw = (y - b) / (r - x);
		CSlopeBrick* slope = (CSlopeBrick*)obj;
		slope->SetRatio(ratio_hw);
	}
	break;
	case OBJECT_TYPE_WATER:
	{
		obj = new CWater();
	}
	break;
	case OBJECT_TYPE_WINDOW:
	{
		obj = new CWindow();
	}
	break;
	case OBJECT_TYPE_ZONE:
	{
		obj = new CZone();
	}
	break;
	case OBJECT_TYPE_ELEVATOR:
	{
		obj = new CElevator();
	}
	break;
	case OBJECT_TYPE_TREASUE:
	{
		int type = atoi(tokens[5].c_str());
		obj = new CTreasure(type);
	}
	break;
	case OBJECT_TYPE_PORTAL:
	{
		int scene_id = atoi(tokens[5].c_str());
		obj = new CPortal(x, y, scene_id);
	}
	break;
	case OBJECT_TYPE_HUD:
	{
		hud = new CHud();
	
	}
	break;
	case OBJECT_TYPE_BLACKMONSTER:
	{
		obj = new CBlackMonster(x);	
	}
	break;
	case OBJECT_TYPE_BOSS:
	{
		obj = new CBoss(x);
	}
	break;
	case OBJECT_TYPE_BALLGUN:
	{
		obj = new CBallGun();

	}
	break;
	case OBJECT_TYPE_TIMEBOMB:
	{
		obj = new CTimeBomb();

	}
	break;
	case OBJECT_TYPE_TUBE:
	{
		float type = atof(tokens[5].c_str());
		obj = new CTube(type);
	}
	break;	
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	if (obj != NULL)
	{
		obj->SetId(objId);
		obj->SetPosition(x, y);
		obj->SetRespawnPosition(x, y);

		/*LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);*/

		if (ani_set_id != -1)
			obj->SetAnimationSet(ani_set);

		if (!dynamic_cast<CYumetaro*>(obj))
			objects.push_back(obj);
	}
	if (player != NULL)
	{
		float cx, cy;
		player->GetPosition(cx, cy);
		CCamera::GetInstance()->SetPosition(cx, cy);
		//hud->SetPosition(cx, cy - CGame::GetInstance()->GetScreenHeight());
	}

}

void CPlayScene::_ParseSection_MAP(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return;

	int idTexMap = atoi(tokens[1].c_str());
	float mapWidth = atoi(tokens[2].c_str());
	float mapHeight = atoi(tokens[3].c_str());
	map = new CTileMap(idTexMap, tokens[0]);

	CCamera::GetInstance()->SetMapSize(0, TOP_MAP_1_1, RIGHT_MAP_1_1, BOTTOM_MAP_1_1, WIDTH_MAP_1_1, HEIGHT_MAP_1_1);
}

void CPlayScene::_ParseSection_GRID(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	grid = new CGrid(WIDTH_ALL_MAP_1_1, HEIGHT_ALL_MAP_1_1, CELL_SIZE);

	ifstream f;
	f.open(tokens[0]);

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		vector<string> tokens = split(line, " ");

		if (tokens.size() < 1) return;

		int cellIndex = 0;
		int objIndex = 0;

		cellIndex = atoi(tokens[0].c_str());
		for (int i = 1; i < tokens.size(); i++)
		{
			objIndex = atoi(tokens[i].c_str());
			for (int t = 0; t < objects.size(); t++)
			{
				if (objects[t]->id == objIndex)
				{
					// Add obj into cell of grid
					grid->AddObjToCell(cellIndex, objects[t]);
					objects[t]->listCellIndex.push_back(cellIndex);
					break;
				}
			}
		}
	}

	f.close();
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line == "[MAP]") {
			section = SCENE_SECTION_MAP; continue;
		}
		if (line == "[GRID]") {
			section = SCENE_SECTION_GRID; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		case SCENE_SECTION_MAP: _ParseSection_MAP(line); break;
		case SCENE_SECTION_GRID: _ParseSection_GRID(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);


	// Create quadtree
	quadtree = new Quadtree(1, 0.0f, 768.0f, 2048.0f, 0.0f);

}

void CPlayScene::Update(DWORD dt)
{
	//DebugOut(L"[Yumetaro objects_size]: %d\n", objects.size());
	
	for (size_t i = 0; i < objects.size(); i++) {
		if (/*objects[i]->isActive && !objects[i]->isDie && */!objects[i]->isDead) {
			if (dynamic_cast<CYumetaro*>(objects[i]))
				continue;
			quadtree->Insert(objects[i]);
		}
		
	}
	
	for (size_t i = 0; i < objects.size(); i++)
	{
		if (!objects[i]->isDead && CCamera::GetInstance()->InCamera(objects[i])) {
			if (dynamic_cast<CYumetaro*>(objects[i]))
				continue;
			if (dynamic_cast<CGround*>(objects[i]))
				continue;
			if (dynamic_cast<CSlopeBrick*>(objects[i]))
				continue;
			vector<LPGAMEOBJECT> coObjectsUpdate;
			quadtree->Retrieve(&coObjectsUpdate, objects[i]);
			objects[i]->Update(dt, &coObjectsUpdate);
		}	
	}
	vector<LPGAMEOBJECT> coObjects;
	// Update player
	quadtree->Retrieve(&coObjects, player);
	//DebugOut(L"[Yumetaro CoObjects]: %d\n", coObjects.size());
	player->Update(dt, &coObjects);

	// Làm trống quadtree
	quadtree->Clear();
	if (player == NULL) return;

	hud->Update(dt);
	CCamera::GetInstance()->Update(dt);
}

void CPlayScene::Render()
{
	if (map != NULL)
	{
		map->Render();
	}
	// Render objects
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->isActive && !objects[i]->isDead  && CCamera::GetInstance()->InCamera(objects[i])) {
			if (dynamic_cast<CYumetaro*>(objects[i]))
				continue;
			else objects[i]->Render();
		}	
	}
	if (player) player->Render();
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->isActive  && !objects[i]->isDead) {
			if (dynamic_cast<CTube*>(objects[i]) || dynamic_cast<CWindow*>(objects[i]) || dynamic_cast<CBlackPipe*>(objects[i]))
				objects[i]->Render();
		}
	}

	hud->Render();
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	// view objs	
	viewOtherObjs.clear();
	viewObjs.clear();
	viewAfterObjs.clear();

	// behind objs
	for (int i = 0; i < behindObjs.size(); i++)
		delete behindObjs[i];
	behindObjs.clear();

	// front objs
	for (int i = 0; i < frontObjs.size(); i++)
		delete frontObjs[i];
	frontObjs.clear();

	// objs
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];
	objects.clear();

	delete player;
	player = NULL;
	if (quadtree) {
		delete quadtree;
		quadtree = NULL;
	}
	delete grid;
	delete map;
	delete hud;
	grid = NULL;
	map = NULL;
	hud = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScene::EndScene()
{
	hud->EndScene();
}
void CPlayScene::PushBackObj(CGameObject* obj) {
	objects.push_back(obj);
}
void CPlayScene::ChangeYumetaroLocation(int childSceneId, bool isCameraStatic, float x, float y)
{
	player->SetPosition(x, y);

	CCamera::GetInstance()->SetIsStatic(isCameraStatic);
	/*switch (map->GetId())
	{
	case MAP_1:
		CCamera::GetInstance()->SetMapSize(LEFT_OTHER_MAP_1_1, TOP_OTHER_MAP_1_1, RIGHT_OTHER_MAP_1_1, BOTTOM_OTHER_MAP_1_1, WIDTH_OTHER_MAP_1_1, HEIGHT_OTHER_MAP_1_1);
		break;
	case MAP_2:
		CCamera::GetInstance()->SetMapSize(LEFT_OTHER_MAP_1_4, TOP_OTHER_MAP_1_4, RIGHT_OTHER_MAP_1_4, BOTTOM_OTHER_MAP_1_4, WIDTH_OTHER_MAP_1_4, HEIGHT_OTHER_MAP_1_4);
		break;
	default:
		break;
	}*/
	ChangeChildScene(childSceneId);
}

void CPlayScene::ChangeChildScene(int childSceneId)
{
	this->childSceneId = childSceneId;
	switch (childSceneId)
	{
	case MAP_1_0:
		CCamera::GetInstance()->SetMapSize(LEFT_MAP_1_0, TOP_MAP_1_0, RIGHT_MAP_1_0, BOTTOM_MAP_1_0, WIDTH_MAP_1_0, HEIGHT_MAP_1_0);
		break;
	case MAP_1_1:
		CCamera::GetInstance()->SetMapSize(LEFT_MAP_1_1, TOP_MAP_1_1, RIGHT_MAP_1_1, BOTTOM_MAP_1_1, WIDTH_MAP_1_1, HEIGHT_MAP_1_1);
		break;
	case MAP_1_2:
		CCamera::GetInstance()->SetMapSize(LEFT_MAP_1_2, TOP_MAP_1_2, RIGHT_MAP_1_2, BOTTOM_MAP_1_2, WIDTH_MAP_1_2, HEIGHT_MAP_1_2);
		break;
	case MAP_1_3:
		CCamera::GetInstance()->SetMapSize(LEFT_MAP_1_3, TOP_MAP_1_3, RIGHT_MAP_1_3, BOTTOM_MAP_1_3, WIDTH_MAP_1_3, HEIGHT_MAP_1_3);
		break;
	case MAP_1_4:
		CCamera::GetInstance()->SetMapSize(LEFT_MAP_1_4, TOP_MAP_1_4, RIGHT_MAP_1_4, BOTTOM_MAP_1_4, WIDTH_MAP_1_4, HEIGHT_MAP_1_4);
		break;
	case MAP_1_5:
		CCamera::GetInstance()->SetMapSize(LEFT_MAP_1_5, TOP_MAP_1_5, RIGHT_MAP_1_5, BOTTOM_MAP_1_5, WIDTH_MAP_1_5, HEIGHT_MAP_1_5);
		break;
	case MAP_1_6:
		CCamera::GetInstance()->SetMapSize(LEFT_MAP_1_6, TOP_MAP_1_6, RIGHT_MAP_1_6, BOTTOM_MAP_1_6, WIDTH_MAP_1_6, HEIGHT_MAP_1_6);
		break;
	case MAP_1_7:
		CCamera::GetInstance()->SetMapSize(LEFT_MAP_1_7, TOP_MAP_1_7, RIGHT_MAP_1_7, BOTTOM_MAP_1_7, WIDTH_MAP_1_7, HEIGHT_MAP_1_7);
		break;
	case MAP_1_8:
		CCamera::GetInstance()->SetMapSize(LEFT_MAP_1_8, TOP_MAP_1_8, RIGHT_MAP_1_8, BOTTOM_MAP_1_8, WIDTH_MAP_1_8, HEIGHT_MAP_1_8);
		break;
	case MAP_1_9:
		CCamera::GetInstance()->SetMapSize(LEFT_MAP_1_9, TOP_MAP_1_9, RIGHT_MAP_1_9, BOTTOM_MAP_1_9, WIDTH_MAP_1_9, HEIGHT_MAP_1_9);
		break;
	default:
		break;
	}
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	/*DebugOut(L"[KEYDOWN] KeyDown: %d\n", KeyCode);*/
	CTileMap* map = ((CPlayScene*)scence)->GetMap();
	CCamera* camera = CCamera::GetInstance();
	CYumetaro* yumetaro = ((CPlayScene*)scence)->GetPlayer();
	if (yumetaro->state == YUMETARO_STATE_DIE) return;

	switch (KeyCode)
	{
	case DIK_B:
		switch (map->GetId())
		{
		case MAP_1:
			CGame::GetInstance()->SwitchScene(MAP_2);
			camera->SetIsMoving(true);
			break;
		case MAP_2:
			CGame::GetInstance()->SwitchScene(MAP_1);
			break;
		default:
			break;
		}
		break;
		/*case DIK_C:
			CGame::GetInstance()->SwitchScene(WORLD_MAP_1);
			break;*/
	case DIK_V:
		CGame::GetInstance()->SwitchScene(TITLE_SCREEN);
		CCamera::GetInstance()->SetPosition(CGame::GetInstance()->GetScreenWidth() / 2, CGame::GetInstance()->GetScreenHeight() / 2);
		CCamera::GetInstance()->SetMapSize(LEFT_TITLE_SCENE, TOP_TITLE_SCENE, RIGHT_TITLE_SCENE, BOTTOM_TITLE_SCENE, WIDTH_TITLE_SCENE, HEIGHT_TITLE_SCENE);
		break;
	case DIK_NUMPAD0:
		switch (map->GetId())
		{
		case MAP_1:
			((CPlayScene*)scence)->ChangeYumetaroLocation(MAP_1_0, true, 138, 658);
			break;
		case MAP_2:
			/*((CPlayScene*)scence)->ChangeMarioLocation(true, 50, 200);
			camera->SetIsMoving(false);*/
			break;
		default:
			break;
		}
		break;
	case DIK_NUMPAD1:
		switch (map->GetId())
		{
		case MAP_1:
			((CPlayScene*)scence)->ChangeYumetaroLocation(MAP_1_1, true, 895, 500);
			break;
		case MAP_2:
			/*((CPlayScene*)scence)->ChangeMarioLocation(true, YUMETARO_1_4_X_1, YUMETARO_1_4_Y_1);
			camera->SetIsMoving(true);*/
			break;
		default:
			break;
		}
		break;
	case DIK_NUMPAD2:
		switch (map->GetId())
		{
		case MAP_1:
			((CPlayScene*)scence)->ChangeYumetaroLocation(MAP_1_2, true, 569, 294);
			break;
		case MAP_2:
			/*((CPlayScene*)scence)->ChangeMarioLocation(true, YUMETARO_1_4_X_2, YUMETARO_1_4_Y_2);
			camera->SetIsMoving(true);*/
			break;
		default:
			break;
		}
		break;
	case DIK_NUMPAD3:
		switch (map->GetId())
		{
		case MAP_1:
			((CPlayScene*)scence)->ChangeYumetaroLocation(MAP_1_3, true, 538, 100);
			break;
		case MAP_2:
			/*((CPlayScene*)scence)->ChangeMarioLocation(true, YUMETARO_1_4_X_3, YUMETARO_1_4_Y_3);
			camera->SetIsMoving(true);*/
			break;
		default:
			break;
		}
		break;
	case DIK_NUMPAD4:
		switch (map->GetId())
		{
		case MAP_1:
			((CPlayScene*)scence)->ChangeYumetaroLocation(MAP_1_3, true, 1160, 58);
			break;
		case MAP_2:
			/*((CPlayScene*)scence)->ChangeMarioLocation(true, YUMETARO_1_4_X_4, YUMETARO_1_4_Y_4);
			camera->SetIsMoving(true);*/
			break;
		default:
			break;
		}
		break;
	case DIK_NUMPAD5:
		switch (map->GetId())
		{
		case MAP_1:
			((CPlayScene*)scence)->ChangeYumetaroLocation(MAP_1_5, true, 1977, 52);
			break;
		case MAP_2:
			/*((CPlayScene*)scence)->ChangeMarioLocation(true, YUMETARO_1_4_X_4, YUMETARO_1_4_Y_4);
			camera->SetIsMoving(true);*/
			break;
		default:
			break;
		}
		break;
	case DIK_NUMPAD6:
		switch (map->GetId())
		{
		case MAP_1:
			((CPlayScene*)scence)->ChangeYumetaroLocation(MAP_1_6, true, 1812, 342);
			break;
		case MAP_2:
			/*((CPlayScene*)scence)->ChangeMarioLocation(true, YUMETARO_1_4_X_5, YUMETARO_1_4_Y_5);
			camera->SetIsMoving(true);*/
			break;
		default:
			break;
		}
		break;
	case DIK_NUMPAD7:
		switch (map->GetId())
		{
		case MAP_1:
			((CPlayScene*)scence)->ChangeYumetaroLocation(MAP_1_7, true, 2004, 526);
			break;
		case MAP_2:
			/*((CPlayScene*)scence)->ChangeMarioLocation(true, YUMETARO_1_4_X_6, YUMETARO_1_4_Y_6);
			camera->SetIsMoving(true);*/
			break;
		default:
			break;
		}
		break;
	case DIK_NUMPAD8:
		switch (map->GetId())
		{
		case MAP_1:
			((CPlayScene*)scence)->ChangeYumetaroLocation(MAP_1_8, true, 1950, 636);
			break;
		case MAP_2:
			/*((CPlayScene*)scence)->ChangeMarioLocation(true, YUMETARO_1_4_X_7, YUMETARO_1_4_Y_7);
			camera->SetIsMoving(true);*/
			break;
		default:
			break;
		}
		break;
	case DIK_NUMPAD9:
		switch (map->GetId())
		{
		case MAP_1:
			((CPlayScene*)scence)->ChangeYumetaroLocation(MAP_1_9, true, 1226, 634);
			break;
		case MAP_2:
			break;
		default:
			break;
		}
		break;
	case DIK_Z:
		yumetaro->canAttack = true;
		break;
	case DIK_S:
		if (yumetaro->canJump)
			yumetaro->vy = YUMETARO_JUMP_Y_SPEED;
		yumetaro->canRepeatJump = false;
		//mario->canJump = false;		
		break;
	case DIK_A:
		
		yumetaro->canHold = true;	
		if (yumetaro->canAttack) {
			DebugOut(L"Yumetaro Attack n %d\n");
			CStar* bullet = new CStar();
			((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->PushBackObj(bullet);
			bullet->SetPosition(yumetaro->x, yumetaro->top + 16);
			bullet->firstY = yumetaro->top + 16;
			bullet->vx = yumetaro->nx * STAR_ENEMY_X_SPEED * 5;
			bullet->vy = STAR_Y_SPEED / 7;
			bullet->SetState(STAR_STATE_FIRE);
			yumetaro->canAttack = false;
			
		}
		break;
	case DIK_R:
		yumetaro->Reset();
		break;
	}
}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	/*DebugOut(L"[KEYUP] KeyUp: %d\n", KeyCode);*/
	CGame* game = CGame::GetInstance();
	CYumetaro* yumetaro = ((CPlayScene*)scence)->GetPlayer();
	if (yumetaro->state == YUMETARO_STATE_DIE) return;

	switch (KeyCode)
	{
	case DIK_DOWN:
		/*if (!yumetaro->canAttack)
			yumetaro->canAttack = true;*/
		break;
	case DIK_X:
		break;
	/*case DIK_A:
		yumetaro->canAttack = false;
		break;*/
	case DIK_S:
		yumetaro->canJump = false;
		yumetaro->canRepeatJump = true;
		break;
	}
}

void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	CYumetaro* yumetaro = ((CPlayScene*)scence)->GetPlayer();
	int state = yumetaro->GetState();

	// Disable control key when Mario die 
	if (state == YUMETARO_STATE_DIE) {
		return;
	}
	if (game->IsKeyDown(DIK_RIGHT) && state != YUMETARO_STATE_PIPE) {
		yumetaro->nx = 1;
		// Increase speed
		if (yumetaro->vx < YUMETARO_WALKING_SPEED)
			yumetaro->vx += YUMETARO_INCREASE_X_SPEED;

		// Skid
		if (yumetaro->vx < 0 && yumetaro->vx < -YUMETARO_CAN_SKID_X_SPEED && yumetaro->isOnGround && !yumetaro->isHold)
		{

		}
		else if (yumetaro->vx >= 0 && yumetaro->isOnGround)
			yumetaro->SetState(YUMETARO_STATE_WALK);
	}
	// Key Left
	else if (game->IsKeyDown(DIK_LEFT) && state != YUMETARO_STATE_PIPE)
	{
		yumetaro->nx = -1;
		// Increase speed
		if (yumetaro->vx > -YUMETARO_WALKING_SPEED)
			yumetaro->vx -= YUMETARO_INCREASE_X_SPEED;

		// Skid
		if (yumetaro->vx > 0 && yumetaro->vx > YUMETARO_CAN_SKID_X_SPEED && yumetaro->isOnGround && !yumetaro->isHold)
		{
		}
		else if (yumetaro->vx <= 0 && yumetaro->isOnGround)
			yumetaro->SetState(YUMETARO_STATE_WALK);
	}
	// When release key Right/Left
	else if (yumetaro->isOnGround && !yumetaro->isColSlopeBrick && !yumetaro->isExplode && yumetaro->kick_start == 0 && yumetaro->tail_start == 0)
	{
		if (yumetaro->vx >= 0)
		{
			yumetaro->SetState(YUMETARO_STATE_WALK);
			yumetaro->vx -= YUMETARO_DECREASE_X_SPEED;
			if (yumetaro->vx <= 0)
			{
				yumetaro->SetState(YUMETARO_STATE_IDLE);
				//DebugOut(L"[release]: right %d\n");
			}

		}
		if (yumetaro->vx < 0)
		{
			yumetaro->SetState(YUMETARO_STATE_WALK);
			yumetaro->vx += YUMETARO_DECREASE_X_SPEED;
			if (yumetaro->vx >= 0)
			{
				yumetaro->SetState(YUMETARO_STATE_IDLE);
				DebugOut(L"[release]: left %d\n");
			}
		}
	}

	// Key S handle jump
	if (game->IsKeyDown(DIK_S) && yumetaro->canJump && yumetaro->state != YUMETARO_STATE_PIPE )
	{
		if (yumetaro->canJumpHigher)
		{
			yumetaro->vy += YUMETARO_JUMP_HIGH_SPEED_Y;
		}
		//yumetaro->vy -= YUMETARO_JUMP_HIGH_SPEED_Y;
		yumetaro->SetState(YUMETARO_STATE_JUMP);
	}
}