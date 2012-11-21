#include "ViewPort.h"

ViewPort* curViewPort;

ViewPort::ViewPort(void)
{
	innerMatrixGenerator = new MatrixGenerator();
	Position = innerMatrixGenerator->Position;
	Rotation = innerMatrixGenerator->Rotation;

	ProjectrionMatrix = perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.f);
}


ViewPort::~ViewPort(void)
{
	delete innerMatrixGenerator;
}

void ViewPort::Update()
{
	innerMatrixGenerator->Update();
	mat4 inversed = inverse(innerMatrixGenerator->Matrix);
	ViewProjectionMatrix = ProjectrionMatrix * inversed;

	//mat4 orientation = innerMatrixGenerator->Matrix;
	//orientation[3] = vec4(0.0f, 0.0f,0.0f,1.0f);

	fwd = vec3(innerMatrixGenerator->Matrix * vec4(0.0f, 0.0f, -1.0f, 0.0f));
	right = vec3(innerMatrixGenerator->Matrix * vec4(1.0f, 0.0f, 0.0f, 0.0f));

	smoothMove = smoothMove * 0.99f + (movingVec.x * right + movingVec.z * fwd) / 1000.0f;
	*Position += smoothMove;
}

void ViewPort::Bind()
{
	curViewPort = this;
}