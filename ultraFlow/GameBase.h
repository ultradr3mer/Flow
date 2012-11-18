#include "Base.h"

#ifndef _BASE_H_
    #define _BASE_H_

class GameBase
{
protected:
	GLfloat frameDuration;
public:
	vec3* Position;
	vec3* Rotation;
	GameBase();
	virtual void Update(void);
};

#endif