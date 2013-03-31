#pragma once
#include "Base.h"
struct Ray
{
	vec3 Origin;
	vec3 Destination;
	bool Hit;
	vec2 HitCoords;
	float HitMult;
	float Threshold;

	// 3D Equasion
	inline Ray(vec3 origin, vec3 dest)
	{
		Origin = origin;
		Destination = dest;
		Hit = false;
		Threshold = 0.001f;
	}

	// 2D Equasion
	inline Ray(vec2 origin, vec2 dest)
	{
		Origin = vec3(origin,0.0f);
		Destination = vec3(dest,0.0f);
		Hit = false;
		Threshold = 0.001f;
	}

	inline Ray()
	{
		Threshold = 0.001f;
		Hit = false;
	}

	inline vec3 HitPos()
	{
		return Origin * (1 - HitMult) + Destination * HitMult;
	}
};
