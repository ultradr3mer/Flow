#include "ViewPort.h"
#include "GenFunc.h"

PlnEquation::PlnEquation(vec3 point1, vec3 point2, vec3 point3)
{
	n = normalize(cross(point2-point1,point3-point1));
	d = dot(point1,n);
}

bool PlnEquation::Check(vec3 point, float bias)
{
	return dot(point,n) < d + bias;
}

ViewPort* curViewPort;

ViewPort::ViewPort(void)
{
	Near = 0.1f;
	Far = 100;
	FovY = 45.0f/360.0f;

	Aspect = (float)screenX / (float)screenY;
	CalcReconstrucVecs = false;

	BuildPerspective();
}


ViewPort::~ViewPort(void)
{
	GameObjList.Remove(this);
}

void ViewPort::BuildPerspective()
{
	ProjectrionMatrix = perspective(FovY*360.0f, Aspect, Near, Far);
}

void ViewPort::Update()
{
	if(wasUpdated())
	{
		InvViewMat = MatrixFromPosAng(Position,Rotation);
		ViewMat = inverse(InvViewMat);
		ViewProjectionMatrix = ProjectrionMatrix * ViewMat;

		InvViewProjectionMatrix = inverse(ViewProjectionMatrix);

		//fwd = vec3(ViewMat * vec4(0.0f, 0.0f, -1.0f, 0.0f));
		//right = vec3(ViewMat * vec4(1.0f, 0.0f, 0.0f, 0.0f));

		if(CalcReconstrucVecs)
		{
			vec4 posCenter = InvViewProjectionMatrix * vec4(0.0f, 0.0f, 1.0f, 1.0f);
			posCenter /= posCenter.w;
			vec4 posRight = InvViewProjectionMatrix * vec4(1.0f, 0.0f, 1.0f, 1.0f);
			posRight /= posRight.w;
			vec4 posUp = InvViewProjectionMatrix * vec4(0.0f, 1.0f, 1.0f, 1.0f);
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
			fwd = vec3(InvViewMat * vec4(0.0f, 0.0f, 1.0f, 0.0f));
			right = vec3(InvViewMat * vec4(1.0f, 0.0f, 0.0f, 0.0f));
			up = vec3(InvViewMat * vec4(0.0f, 1.0f, 0.0f, 0.0f));
		}

		#pragma region frustum culling generation

		vec4 edge000 = InvViewProjectionMatrix * vec4(-1.0f, -1.0f, -1.0f, 1.0f);
		vec4 edge001 = InvViewProjectionMatrix * vec4(-1.0f, -1.0f, 1.0f, 1.0f);
		vec4 edge010 = InvViewProjectionMatrix * vec4(-1.0f, 1.0f, -1.0f, 1.0f);
		vec4 edge011 = InvViewProjectionMatrix * vec4(-1.0f, 1.0f, 1.0f, 1.0f);

		vec4 edge100 = InvViewProjectionMatrix * vec4(1.0f, -1.0f, -1.0f, 1.0f);
		vec4 edge101 = InvViewProjectionMatrix * vec4(1.0f, -1.0f, 1.0f, 1.0f);
		vec4 edge110 = InvViewProjectionMatrix * vec4(1.0f, 1.0f, -1.0f, 1.0f);

		edge000 /= edge000.w;
		edge001 /= edge001.w;
		edge010 /= edge010.w;
		edge011 /= edge011.w;

		edge100 /= edge100.w;
		edge101 /= edge101.w;
		edge110 /= edge110.w;

		clipingPlanes[0] = PlnEquation(vec3(edge000),vec3(edge010),vec3(edge001));
		clipingPlanes[1] = PlnEquation(vec3(edge100),vec3(edge101),vec3(edge110));

		clipingPlanes[2] = PlnEquation(vec3(edge000),vec3(edge100),vec3(edge010));
		clipingPlanes[3] = PlnEquation(vec3(edge001),vec3(edge011),vec3(edge101));

		clipingPlanes[4] = PlnEquation(vec3(edge000),vec3(edge001),vec3(edge100));
		clipingPlanes[5] = PlnEquation(vec3(edge010),vec3(edge110),vec3(edge011));

		#pragma endregion
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

int DrawnObj;
int NotDrawnObj;

bool ViewPort::FrustumCheck(vec3 position, float radius)
{
	//float test = length(fwd);
	//test = length(up);
	//test = length(right);

	//vec3 shift = radius * (up + right);
	//vec4 SSPos1 = ViewProjectionMatrix * vec4(position + shift + radius * fwd,1.0f);
	//SSPos1 /= SSPos1.w;
	//vec4 SSPos2 = ViewProjectionMatrix * vec4(position - shift + radius * fwd,1.0f);
	//SSPos2 /= SSPos2.w;

	//if(
	//	((SSPos1.x < 1 && SSPos1.x > -1) || (SSPos2.x < 1 && SSPos2.x > -1) || (sign(SSPos1.x) != sign(SSPos2.x))) &&
	//	((SSPos1.y < 1 && SSPos1.y > -1) || (SSPos2.y < 1 && SSPos2.y > -1) || (sign(SSPos1.y) != sign(SSPos2.y))))
	//{
	//	vec4 SSPos3 = ViewProjectionMatrix * vec4(position - radius * fwd,1.0f);
	//	SSPos3 /= SSPos3.w;
	//	if((SSPos1.z < 1 && SSPos1.z > -1) || (SSPos3.z < 1 && SSPos3.z > -1) || (sign(SSPos1.z) != sign(SSPos3.z)))
	//	{
	//		DrawnObj ++;
	//		return true;
	//	}
	//}
	//NotDrawnObj ++;
	//return false;

	for (int i = 0; i < 6; i++)
	{
		if(!clipingPlanes[i].Check(position,radius))
		{
			NotDrawnObj ++;
			return false;
		}
	}
	DrawnObj ++;
	return true;
}