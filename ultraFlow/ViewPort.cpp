#include "ViewPort.h"
#include "GenFunc.h"

ViewPort* curViewPort;

ViewPort::ViewPort(void)
{
	Near = 0.1f;
	Far = 100;
	Aspect = (float)screenX / (float)screenY;
	ProjectrionMatrix = perspective(45.0f, Aspect, Near, Far);
	CalcReconstrucVecs = false;
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

		if(CalcReconstrucVecs)
		{
			mat4 invViewProjectionMatrix = inverse(ViewProjectionMatrix);
			vec4 posCenter = invViewProjectionMatrix * vec4(0.0f, 0.0f, 1.0f, 1.0f);
			posCenter /= posCenter.w;
			vec4 posRight = invViewProjectionMatrix * vec4(1.0f, 0.0f, 1.0f, 1.0f);
			posRight /= posRight.w;
			vec4 posUp = invViewProjectionMatrix * vec4(0.0f, 1.0f, 1.0f, 1.0f);
			posUp /= posUp.w;

			fwdD = vec3(posCenter) - Position;
			rightD = vec3(posRight - posCenter);
			upD = vec3(posUp - posCenter);

			fwd = normalize(fwdD);
			right = normalize(rightD);
			up = normalize(upD);
		}
		else
		{
			fwd = vec3(viewMat * vec4(0.0f, 0.0f, 1.0f, 0.0f));
			right = vec3(viewMat * vec4(1.0f, 0.0f, 0.0f, 0.0f));
			up = vec3(viewMat * vec4(0.0f, 1.0f, 0.0f, 0.0f));
		}
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