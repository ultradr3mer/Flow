#include "PhysicsModel.h"
#include "ViewPort.h"
#include "BulletManager.h"

PhysicsModel::PhysicsModel(void)
{
}


PhysicsModel::~PhysicsModel(void)
{
	dynamicsWorld->removeRigidBody(Body);
}

void PhysicsModel::Update(void)
{
	if(Body->isActive())
		Body->getWorldTransform().getOpenGLMatrix(m);
}

void PhysicsModel::setupMatrices()
{
	ShaderData::UniformMatrix4fv(MatModelView, m);
	ShaderData::UniformMatrix4fv(MatViewProjection, curViewPort->ViewProjectionMatrix);
}