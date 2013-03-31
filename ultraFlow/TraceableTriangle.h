#pragma once
#include "Base.h"
#include "Ray.h"
#include "PlnEquation.h"

const vec2 ownPointsTs[] = {
	vec2(0,0),
	vec2(1,0),
	vec2(0,1)
};

//const vec2 ownPointsSmall[] = {
//	vec2(0.001f,0.001f),
//	vec2(0.999f,0.001f),
//	vec2(0.001f,0.999f)
//};

//const vec2 ownPointsSmall[] = {
//	vec2(0.1f,0.1f),
//	vec2(0.9f,0.1f),
//	vec2(0.1f,0.9f)
//};

//const mat4 swizzleAxes = mat4(
//	0.0f, 1.0f, 0.0f, 0.0f,
//	0.0f, 0.0f, 1.0f, 0.0f,
//	1.0f, 0.0f, 0.0f, 0.0f,
//	0.0f, 0.0f, 0.0f, 1.0f
//);

const mat4 swizzleAxes = mat4(
	0.0f, 0.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
);

struct TraceableTriangle
{
	vec3 Point[3];
	vec3 Center;
	float BoundingSphere;
	float Threshold;

	mat4 LocalisationMatrix;
	PlnEquation Pln;

	TraceableTriangle(vec3 point1, vec3 point2, vec3 point3);
	TraceableTriangle(vec3 point[3]);
	~TraceableTriangle(void);

	virtual void RayTest(Ray* ray);
	virtual bool PointTest(vec3 dot);
	void PlaneInternTesting(Ray* ray);
	vec3 TransformToTangentSpace(vec3 point);
	void InitTransformation();
	TraceableTriangle();
	bool IsParallel(Ray* ray, float bias);
	Ray GetIntersectionLine(TraceableTriangle* tri);
	vec3* EvenScaling(float dist);
	bool IntersectWithPlane(PlnEquation* pln, vec3* intersections);
};

//struct TraceableLine2D : TraceableTriangle
//{
//	TraceableLine2D(vec2 point1, vec2 point2);
//	TraceableLine2D();
//	~TraceableLine2D();
//
//	virtual void RayTest(Ray* ray);
//};