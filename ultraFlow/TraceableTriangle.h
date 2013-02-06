#pragma once
#include "Base.h"

const vec2 ownPointsTs[] = {
	vec2(0,0),
	vec2(1,0),
	vec2(0,1)
};

struct Ray
{
	vec3 Origin;
	vec3 Destination;
	bool Hit;
	vec2 HitCoords;
	float HitMult;

	inline Ray(vec3 origin, vec3 dest)
	{
		Origin = origin;
		Destination = dest;
		Hit = false;
	}

	inline Ray()
	{
	}

	inline vec3 HitPos()
	{
		return Origin * (1 - HitMult) + Destination * HitMult;
	}
};

struct TraceableTriangle
{
	vec3 Point[3];
	
	mat4 LocalisationMatrix;

	TraceableTriangle(vec3 point1, vec3 point2, vec3 point3);
	~TraceableTriangle(void);

	void RayTest(Ray* ray);
	vec3 TransformToTangentSpace(vec3 point);
	void InitTransformation();
	inline TraceableTriangle(){}
	bool IsParallel(Ray* ray, float bias);
};

