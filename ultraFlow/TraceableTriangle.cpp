#include "TraceableTriangle.h"
#include "GenFunc.h"

// Constructor
TraceableTriangle::TraceableTriangle(vec3 point1, vec3 point2, vec3 point3)
{
	// Set Basic Data
	Point[0] = point1;
	Point[1] = point2;
	Point[2] = point3;

	// Initialize
	InitTransformation();
}

void TraceableTriangle::InitTransformation()
{
	// Initial Transformation
	LocalisationMatrix = inverse(MatrixFromPosAng(Point[0],RotationFromNormal(Point[1] - Point[0])));
	vec3 localPoint2 = vec3(LocalisationMatrix * vec4(Point[1],1));
	vec3 localPoint3 = vec3(LocalisationMatrix * vec4(Point[2],1));

	// Calculate Addidional Transformation
	float xzScale = 1/length(vec2(localPoint3.x,localPoint3.z));
	float yScale = 1/localPoint2.y;
	float sheerFactor = localPoint3.y * yScale;

	// Genearate Advanced Transformation Matrix

	// Rotate
	LocalisationMatrix = rotate(mat4(1.0f),
		-atan2(localPoint3.x,localPoint3.z) / pi * 180.0f,
		vec3(0.0f,1.0f,0.0f)) * LocalisationMatrix;

	// Scale
	LocalisationMatrix = scale(mat4(1.0f),
		vec3(xzScale,yScale,xzScale)) * 
		LocalisationMatrix;

	// Shear
	mat4 sheerMat = mat4(1.0f);
	sheerMat[2].y = -sheerFactor;
	LocalisationMatrix = sheerMat * LocalisationMatrix;

	////// Test
	//vec3 test1 = TransformToTangentSpace(Point[0]);
	//vec3 test2 = TransformToTangentSpace(Point[1]);
	//vec3 test3 = TransformToTangentSpace(Point[2]);
}

// Basic TransformFunc
vec3 TraceableTriangle::TransformToTangentSpace(vec3 point)
{
	vec4 returnPoint = LocalisationMatrix * vec4(point,1);
	return vec3(returnPoint.y,returnPoint.z,returnPoint.x);
}

// Deconstructor
TraceableTriangle::~TraceableTriangle(void)
{
}

const float threshold = 0.0001f;

// Perform RayTest
void TraceableTriangle::RayTest(Ray* ray)
{
	// Transform equasion to tangent space
	vec3 origin = TransformToTangentSpace(ray->Origin);
	vec3 direction = TransformToTangentSpace(ray->Destination)-origin;

	// get Hitpoint
	float mult = -origin.z / direction.z;
	vec3 hit = origin+direction*mult;

	// return Result
	if(hit.x >= -threshold && hit.y >= -threshold && hit.x + hit.y <= 1+threshold)
	{
		ray->Hit = true;
		ray->HitMult = mult;
		ray->HitCoords = vec2(hit);
	}
	else
	{
		ray->Hit = false;
	}
}

// Check for Parallelity
bool TraceableTriangle::IsParallel(Ray* ray, float bias)
{
	vec3 vecPln = normalize(cross(Point[0]-Point[1],Point[0]-Point[2]));
	vec3 vecGrd = normalize(ray->Destination-ray->Origin);
	float parallelity = dot(vecPln,vecGrd);
	if(parallelity < 0) parallelity *= -1;

	return parallelity < bias;
}