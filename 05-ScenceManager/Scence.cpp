#include "Scence.h"

CScene::CScene(int id, LPCWSTR filePath)
{
	this->id = id;
	if (id == MAP_1) childSceneId = MAP_1_1;
	this->sceneFilePath = filePath;
}