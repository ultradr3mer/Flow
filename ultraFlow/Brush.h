#pragma once
#include "Base.h"
#include "MapMeshData.h"
class Brush
{
private:
	void makeFace(int index0, int index1, int index2, int index3, int textureMapping);
public:
	vec3 Position;
	vec3 Size;
	vec3 Center;
	float TestingSize;
	bool Substract;
	uint Texture;
	ListContainer<MapTriangle> Triangles;
	ListContainer<Brush> IntersectingBrushes;
	ListContainer<SimpleTriangle> SubTriangles;
	bool PerformedIntersections;

	Brush(vec3 position, vec3 size, uint texture, bool substract);

	bool PointTest(vec3 Point);
	void GenerateTriangles();
	void PrepareIntersections(ListContainer<Brush>* brushes);
	void PerformIntersections(ListContainer<SimpleTriangle>* resultingTriangles);
	void CleanupIntersections();
};