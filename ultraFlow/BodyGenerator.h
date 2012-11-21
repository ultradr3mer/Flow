#include "Base.h"

extern btDiscreteDynamicsWorld* dynamicsWorld;

#pragma once
class BodyGenerator
{
public:
	static void Init(void);
	static btRigidBody* FromObj(char* source);
	static btCollisionShape* ShapeFromObj(char* source);
	static void Step();
};

