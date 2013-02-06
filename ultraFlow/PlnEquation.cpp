#include "PlnEquation.h"
PlnEquation::PlnEquation(vec3 point1, vec3 point2, vec3 point3)
{
	n = normalize(cross(point2-point1,point3-point1));
	d = dot(point1,n);
}

bool PlnEquation::Check(vec3 point, float bias)
{
	return dot(point,n) < d + bias;
}

float PlnEquation::Check(vec3 point)
{
	return dot(point,n) - d;
}
