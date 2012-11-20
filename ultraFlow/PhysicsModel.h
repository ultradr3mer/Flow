#include "Base.h"
#include "Model.h"

#pragma once
class PhysicsModel : public Model
{
private:
	GLfloat m[16];
	virtual void setupMatrices(void);
public:
	PhysicsModel(void);
	~PhysicsModel(void);
	virtual void Update(void);
	btRigidBody* Body;
};

