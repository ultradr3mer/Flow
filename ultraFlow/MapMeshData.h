#pragma once
#include "Base.h"
#include "TraceableTriangle.h"
#include "PlnEquation.h"

struct SimpleTriangle
{
	vec3 Point[3];
	vec2 TexCoord[3];

	SimpleTriangle(vec3 point1, vec3 point2, vec3 point3, 
		vec2 texCoord1, vec2 texCoord2, vec2 texCoord3);
};

struct MapTriangle : public TraceableTriangle
{
	vec2 TexCoord[3];

	ListContainer<SimpleTriangle> SubTriangles;

	MapTriangle(vec3 point1, vec3 point2, vec3 point3, 
		vec2 texCoord1, vec2 texCoord2, vec2 texCoord3);

	inline MapTriangle(){}

	void GenerateSubTriangles();
	static void GenerateAllSubTriangles();
	bool IntersectionHelper(MapTriangle tri);
};

extern MapTriangle mapTriangles[128000];
extern uint mapTrianglesLength;