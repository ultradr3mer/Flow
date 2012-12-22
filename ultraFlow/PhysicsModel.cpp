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
	{
		Body->getWorldTransform().getOpenGLMatrix(m);
		mat4 matrix = mat4(
			m[0],m[1],m[2],m[3],
			m[4],m[5],m[6],m[7],
			m[8],m[9],m[10],m[11],
			m[12],m[13],m[14],m[15]
		);

		Position = vec3(matrix * vec4(0,0,0,1));
	}
}

void PhysicsModel::setupMatrices()
{
	ShaderData::UniformMatrix4fv(MatModelView, m);
	ShaderData::UniformMatrix4fv(MatViewProjection, curViewPort->ViewProjectionMatrix);
}