#include "PlayerInfo.h"

CPlayerInfo* CPlayerInfo::__instance = NULL;

CPlayerInfo::CPlayerInfo()
{
	life = LIFE_4;
	money = 0;
	score = 0;	
}

CPlayerInfo* CPlayerInfo::GetInstance()
{
	if (__instance == NULL) __instance = new CPlayerInfo();
	return __instance;
}
