#pragma once
#include "Base.h"
#include "TraceableTriangle.h"
#include "PlnEquation.h"
#include "Line2d.h"

struct SimpleTriangle
{
	vec3 Point[3];
	vec2 TexCoord[3];

	SimpleTriangle(vec3 point1, vec3 point2, vec3 point3, 
		vec2 texCoord1, vec2 texCoord2, vec2 texCoord3);

	SimpleTriangle(vec3 points[3], vec2 texCoords[3]);
	//inline ~SimpleTriangle(){};

	vec3 Center();
	vec3 Normal();
	SimpleTriangle* Copy();
	void SetFront(vec3 normal);
};

struct MapTriangle : public TraceableTriangle
{
	vec2 TexCoord[3];

	ListContainer<SimpleTriangle> SubTriangles;

	MapTriangle(vec3 point1, vec3 point2, vec3 point3, 
		vec2 texCoord1, vec2 texCoord2, vec2 texCoord3);

	inline MapTriangle(){}

	void GenerateSubTriangles(ListContainer<MapTriangle>* intersectionTriangles);
	bool IntersectionHelper(MapTriangle* tri);
};

//extern ListContainer<MapTriangle> mapTriangles;