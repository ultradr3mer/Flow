#include "ViewPort.h"
#include "GenFunc.h"

ViewPort* curViewPort;

ViewPort::ViewPort(void)
{
	Near = 0.1;
	Far = 100;
	Aspect = 4.0f / 3.0f;
	ProjectrionMatrix = perspective(45.0f, Aspect, Near, Far);
}


ViewPort::~ViewPort(void)
{
}

void ViewPort::Update()
{
	if(wasUpdated())
	{
		mat4 viewMat = MatrixFromPosAng(Position,Rotation);
		mat4 inversed = inverse(viewMat);
		ViewProjectionMatrix = ProjectrionMatrix * inversed;

		//fwd = vec3(viewMat * vec4(0.0f, 0.0f, -1.0f, 0.0f));
		//right = vec3(viewMat * vec4(1.0f, 0.0f, 0.0f, 0.0f));

		mat4 invViewProjectionMatrix = inverse(ViewProjectionMatrix);
		vec4 posCenter = invViewProjectionMatrix * vec4(0.0f, 0.0f, 1.0f, 1.0f);
		posCenter /= posCenter.w;
		vec4 posRight = invViewProjectionMatrix * vec4(1.0f, 0.0f, 1.0f, 1.0f);
		posRight /= posRight.w;
		vec4 posUp = invViewProjectionMatrix * vec4(0.0f, 1.0f, 1.0f, 1.0f);
		posUp /= posUp.w;

		fwd = vec3(posCenter) - Position;
		right = vec3(posRight - posCenter);
		up = vec3(posUp - posCenter);
		
		fwd1 = normalize(fwd);
		right1 = normalize(right);
		up1 = normalize(up);
	}
}

bool ViewPort::wasUpdated()
{
	if(posOld != Position || rotOld != Rotation)
	{
		posOld = Position;
		rotOld = Rotation;
		return true;
	}
	return false;
}

void ViewPort::Bind()
{
	curViewPort = this;
}