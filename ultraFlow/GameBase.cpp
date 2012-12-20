#include "GameBase.h"

ListContainer<GameBase> GameObjList;

void GameBase::Update(void)
{
}

GameBase::GameBase()
{
	GameObjList.Add(this);
}

GameBase::~GameBase()
{
	GameObjList.Remove(this);
}
