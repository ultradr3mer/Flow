#pragma once
#include "Base.h"
struct PlnEquation
{
	vec3 n;
	float d;

	PlnEquation(){};
	PlnEquation(vec3 point1, vec3 point2, vec3 point3);

	bool Check(vec3 point, float bias);
	float Check(vec3 point);
};
