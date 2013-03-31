#include "PlnEquation.h"
PlnEquation::PlnEquation(vec3 point1, vec3 point2, vec3 point3)
{
	n = normalize(cross(point2-point1,point3-point1));
	d = dot(point1,n);
	Threshold = 0.01f;
}

PlnEquation::PlnEquation(vec3* points)
{
	n = normalize(cross(points[1]-points[0],points[2]-points[0]));
	d = dot(points[0],n);
	Threshold = 0.01f;
}

bool PlnEquation::Check(vec3 point, float bias)
{
	return dot(point,n) < d + bias;
}

float PlnEquation::Check(vec3 point)
{
	return dot(point,n) - d;
}

bool PlnEquation::RayTest(Ray* ray)
{
	vec3 dir = normalize(ray->Destination-ray->Origin)*Threshold;
	float resultA = Check(ray->Origin-dir);
	float resultB = Check(ray->Destination+dir);

	if(resultA * sign(resultA) < Threshold && 
		resultB * sign(resultB) < Threshold)
		return false;

	if(resultA > 0 != resultB > 0)
	{
		float rayD = dot(ray->Origin,n);
		float a = dot(ray->Destination-ray->Origin,n);
		ray->HitMult = (d-rayD)/a;
		return true;
	}

	return false;
}