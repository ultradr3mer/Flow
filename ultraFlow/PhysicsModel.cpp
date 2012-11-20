#include "PhysicsModel.h"
#include "ViewPort.h"

PhysicsModel::PhysicsModel(void)
{
}


PhysicsModel::~PhysicsModel(void)
{
}

void PhysicsModel::Update(void)
{
	Body->getWorldTransform().getOpenGLMatrix(m);
}

void PhysicsModel::setupMatrices()
{
	ShaderData::UniformMatrix4fv(MatModelView, m);
	ShaderData::UniformMatrix4fv(MatViewProjection, curViewPort->ViewProjectionMatrix);
}