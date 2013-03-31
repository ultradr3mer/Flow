#pragma once
#include "Base.h"
#include "PlnEquation.h"
class Line2d
{
public:
	Line2d(vec2 origin,vec2 destination);
	~Line2d(void);
	bool Intersect(Line2d* line);
	vec2 Origin;
	vec2 Destination;
	float Threshold;
	PlnEquation Pln;
	vec2 intersectionPoint;
private:
};

