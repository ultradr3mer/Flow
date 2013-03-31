#pragma once
#include "Base.h"
#include "Ray.h"

struct PlnEquation
{
	vec3 n;
	float d;
	float Threshold;

	PlnEquation(){Threshold = 0.01f;};
	PlnEquation(vec3 point1, vec3 point2, vec3 point3);
	PlnEquation(vec3* points);

	bool Check(vec3 point, float bias);
	float Check(vec3 point);
	bool RayTest(Ray* ray);
};
