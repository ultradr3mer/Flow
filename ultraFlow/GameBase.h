#pragma once
#include "Base.h"
//#include "ListContainer.h"

class GameBase
{
protected:
	//GLfloat frameDuration;
	//virtual void ExecCleanup();
public:
	vec3 Position;
	vec3 Rotation;
	GameBase();
	virtual ~GameBase();
	virtual void Update(void);
};

extern ListContainer<GameBase> GameObjList;