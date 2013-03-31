#include "Line2d.h"


Line2d::Line2d(vec2 origin,vec2 destination)
{
	Origin = origin;
	Destination = destination;
	Pln = PlnEquation(vec3(origin,0.0f),vec3(destination,0.0f),vec3(origin,1.0f));
	Threshold = 0.01f;
}


Line2d::~Line2d(void)
{
}

bool Line2d::Intersect(Line2d* line)
{
	vec2 dir = normalize(Destination-Origin)*Threshold;
	float resultA = line->Pln.Check(vec3(Origin-dir,0.0f));
	float resultB = line->Pln.Check(vec3(Destination+dir,0.0f));

	if(resultA * sign(resultA) < Threshold && 
		resultB * sign(resultB) < Threshold)
		return false;

	if(resultA > 0 != resultB > 0)
	{
		Ray ray = Ray(line->Origin,line->Destination);
		if(Pln.RayTest(&ray))
		{
			intersectionPoint = vec2(ray.HitPos());
			return true;
		}
	}

	return false;
}