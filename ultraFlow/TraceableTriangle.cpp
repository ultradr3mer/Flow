#include "TraceableTriangle.h"
#include "GenFunc.h"
#include "PlnEquation.h"

// Constructor
TraceableTriangle::TraceableTriangle(vec3 point1, vec3 point2, vec3 point3)
{
	// Set Basic Data
	Point[0] = point1;
	Point[1] = point2;
	Point[2] = point3;

	Threshold = 0.001f;

	// Initialize
	InitTransformation();
}

// Constructor
TraceableTriangle::TraceableTriangle(vec3 point[3])
{
	// Set Basic Data
	Point[0] = point[0];
	Point[1] = point[1];
	Point[2] = point[2];

	Threshold = 0.001f;

	// Initialize
	InitTransformation();
}

TraceableTriangle::TraceableTriangle()
{
	Threshold = 0.001f;
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

	// Swap Axes
	LocalisationMatrix = swizzleAxes * LocalisationMatrix;

	//// Test
	vec3 test1 = TransformToTangentSpace(Point[0]);
	vec3 test2 = TransformToTangentSpace(Point[1]);
	vec3 test3 = TransformToTangentSpace(Point[2]);

	// Calculate Triangle Center
	Center = (Point[0]+Point[1]+Point[2])/3.0f;

	// Calculate BSphere
	BoundingSphere = 0;
	float curDist;
	for (int i = 0; i < 3; i++)
	{
		curDist = length(Point[i]-Center);
		if(curDist > BoundingSphere)
			BoundingSphere = curDist;
	}

	//Prepare Plane
	Pln = PlnEquation(Point);
}

// Basic Transform Function
vec3 TraceableTriangle::TransformToTangentSpace(vec3 point)
{
	vec4 returnPoint = LocalisationMatrix * vec4(point,1);
	//return vec3(returnPoint.y,returnPoint.z,returnPoint.x);
	return vec3(returnPoint);
}

// Deconstructor
TraceableTriangle::~TraceableTriangle(void)
{
}

// Perform RayTest
void TraceableTriangle::RayTest(Ray* ray)
{
	// Transform equasion to tangent space
	vec3 origin = TransformToTangentSpace(ray->Origin);
	vec3 direction = TransformToTangentSpace(ray->Destination)-origin;

	// get Hitpoint
	float mult = -origin.z / direction.z;
	vec3 hit = origin+direction*mult;
	ray->HitMult = mult;
	ray->HitCoords = vec2(hit);

	// return Result
	if(hit.x > -Threshold && hit.y > -Threshold && hit.x + hit.y < 1+Threshold)
	{
		ray->Hit = true;
	}
	else
	{
		ray->Hit = false;
	}
}

bool TraceableTriangle::PointTest(vec3 dot)
{
	// Transform to tangent space
	vec3 point = TransformToTangentSpace(dot);

	// return result
	return (point.x > -Threshold && point.y > -Threshold && point.x + point.y < 1+Threshold);
}

//// Perform raytest for a ray inside the triangle plane
//void TraceableTriangle::PlaneInternTesting(Ray* ray)
//{
//	// Transform equasion to tangent space
//	vec3 origin = TransformToTangentSpace(ray->Origin);
//	vec3 destination = TransformToTangentSpace(ray->Destination);
//
//	// Make the equasion tracable
//	TraceableLine2D equasion = TraceableLine2D(vec2(origin),vec2(destination));
//	equasion.Threshold *= -1.0f;
//
//	// Trace each triangle border
//	int nextIndex;
//	Ray borderRay;
//
//	for (int i = 0; i < 3; i++)
//	{
//		// Ray Origin
//		borderRay.Origin = vec3(ownPointsSmall[i],0);
//
//		// Ray Destination
//		nextIndex = i+1;
//		if(nextIndex == 3)
//			nextIndex = 0;
//		borderRay.Destination = vec3(ownPointsSmall[nextIndex],0);
//
//		equasion.RayTest(&borderRay);
//
//		if(borderRay.Hit && borderRay.HitMult > 0.0f &&
//			borderRay.HitMult < 1.0f)
//		{
//			// return Result
//			ray->Hit = true;
//			ray->HitMult = borderRay.HitCoords.x;
//			//ray->HitCoords = vec2(hit);
//
//			return;
//		}
//	}
//
//	ray->Hit = false;
//}

// Perform raytest for a ray inside the triangle plane
void TraceableTriangle::PlaneInternTesting(Ray* ray)
{
	// Insert Center
	if(PointTest((ray->Destination + ray->Origin)/2.0f))
	{
		ray->Hit = true;
		return;
	}

	//Check if triangle crosses line
	vec3 normal = normalize(cross(
	Point[1]-Point[0],
	Point[2]-Point[0]));
	PlnEquation pln = PlnEquation(ray->Destination,ray->Origin,ray->Destination+normal);

	ray->Hit = false;

	int nextIndex;
	vec3 sidePlanePointA, sidePlanePointB;
	for (int i = 0; i < 3; i++)
	{
		// Point 1
		sidePlanePointA = Point[i];

		// Point 2
		nextIndex = i+1;
		if(nextIndex == 3)
			nextIndex = 0;
		sidePlanePointB = Point[nextIndex];

		// Testing
		float resultA = pln.Check(sidePlanePointA);
		float resultB = pln.Check(sidePlanePointB);
		if((resultA > -Threshold && resultB < Threshold) || 
			(resultA < Threshold && resultB > -Threshold))
		{
			pln = PlnEquation(sidePlanePointA,sidePlanePointB,sidePlanePointB+normal);
			resultA = pln.Check(ray->Destination);
			resultB = pln.Check(ray->Origin);

			if((resultA > Threshold && resultB < -Threshold) || 
			(resultA < -Threshold && resultB > Threshold))
			{
				ray->Hit = true;
			}
		}
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

// Retrive Intersection Line
Ray TraceableTriangle::GetIntersectionLine(TraceableTriangle* tri)
{
	vec3 myIntersections[2], targetIntersections[2], tmpVec, intersectionLine[2];
	float myLinePositions[2], targetLinePositions[2], tmpFlt, intersectionPosition[2];

	Ray ray;
	ray.Hit = false;

	if(length(Center-tri->Center)>(BoundingSphere+tri->BoundingSphere))
		return ray;

	if(IntersectWithPlane(&tri->Pln,myIntersections) &&
		tri->IntersectWithPlane(&Pln,targetIntersections))
	{
		vec3 lineDir = normalize(cross(Pln.n,tri->Pln.n));

		// Find position in line Direction
		myLinePositions[0] = dot(lineDir,myIntersections[0]);
		myLinePositions[1] = dot(lineDir,myIntersections[1]);

		targetLinePositions[0] = dot(lineDir,targetIntersections[0]);
		targetLinePositions[1] = dot(lineDir,targetIntersections[1]);

		// Sort
		if(myLinePositions[1] > myLinePositions[0])
		{
			tmpVec = myIntersections[0];
			tmpFlt = myLinePositions[0];

			myIntersections[0] = myIntersections[1];
			myLinePositions[0] = myLinePositions[1];

			myIntersections[1] = tmpVec;
			myLinePositions[1] = tmpFlt;
		}

		if(targetLinePositions[1] > targetLinePositions[0])
		{
			tmpVec = targetIntersections[0];
			tmpFlt = targetLinePositions[0];

			targetIntersections[0] = targetIntersections[1];
			targetLinePositions[0] = targetLinePositions[1];

			targetIntersections[1] = tmpVec;
			targetLinePositions[1] = tmpFlt;
		}

		// Select 2nd edge
		if(myLinePositions[0] < targetLinePositions[0])
		{
			intersectionLine[0] = myIntersections[0];
			intersectionPosition[0] = myLinePositions[0];
		}
		else
		{
			intersectionLine[0] = targetIntersections[0];
			intersectionPosition[0] = targetLinePositions[0];
		}

		if(myLinePositions[1] > targetLinePositions[1])
		{
			intersectionLine[1] = myIntersections[1];
			intersectionPosition[1] = myLinePositions[1];
		}
		else
		{
			intersectionLine[1] = targetIntersections[1];
			intersectionPosition[1] = targetLinePositions[1];
		}

		// Intersect?
		if(intersectionPosition[0] > intersectionPosition[1])
		{
			ray.Origin = intersectionLine[0];
			ray.Destination = intersectionLine[1];

			ray.Hit = true;
		}
	}

	//vec3 dirToPln = normalize(cross(lineDir,tri->Pln.n));

	//Ray ray = Ray();
	//ray.Origin = tri->Center;
	//ray.Destination = tri->Center+dirToPln;

	//Pln.RayTest(

	return ray;
}

bool TraceableTriangle::IntersectWithPlane(PlnEquation* pln, vec3* intersections)
{
	int nextIndex;
	vec3 edgePointA, edgePointB;
	int returnIntersectionsPos = 0;
	Ray ray;

	for (int i = 0; i < 3; i++)
	{
		// Point 1
		ray.Origin = Point[i];

		// Point 2
		nextIndex = i+1;
		if(nextIndex == 3)
			nextIndex = 0;
		ray.Destination = Point[nextIndex];

		// Testing
		if(pln->RayTest(&ray))
		{
			intersections[returnIntersectionsPos++] = ray.HitPos();
		}
	}

	return returnIntersectionsPos == 2;
}

// Creates Triangle with edges "dist" away from the original ones
vec3 evenScalingReturnAry[3];
vec3* TraceableTriangle::EvenScaling(float dist)
{
	int nextIndex,prevIndex;
	vec3 curPoint, oppositeA, oppositeB;
	vec3 edgeVecA, edgeVecB, shiftVec;
	float angle;

	for (int i = 0; i < 3; i++)
	{
		// Current Point
		curPoint = Point[i];

		// Point A
		nextIndex = i+1;
		if(nextIndex == 3)
			nextIndex = 0;
		oppositeA = Point[nextIndex];

		// Point B
		prevIndex = i-1;
		if(prevIndex == -1)
			prevIndex = 2;
		oppositeB = Point[prevIndex];

		edgeVecA = normalize(oppositeA - curPoint);
		edgeVecB = normalize(oppositeB - curPoint);

		// Get direction
		shiftVec = normalize(edgeVecA + edgeVecB);

		// Multply by length
		angle = acos(dot(edgeVecA,edgeVecB))*0.5f;
		shiftVec *= dist/sin(angle);

		// Save shifted Vertex
		evenScalingReturnAry[i] = curPoint-shiftVec;
	}

	return evenScalingReturnAry;
}

//
//// Constructor
//TraceableLine2D::TraceableLine2D(vec2 point1, vec2 point2)
//{
//	// Set Basic Data
//	Point[0] = vec3(point1,0);
//	Point[1] = vec3(point2,0);
//	Point[2] = vec3(point1,1.0f);
//
//	// Initialize
//	InitTransformation();
//}
//
//// Constructor
//TraceableLine2D::TraceableLine2D()
//{
//}
//
//// Perform RayTest
//void TraceableLine2D::RayTest(Ray* ray)
//{
//	// Transform equasion to tangent space
//	vec3 origin = TransformToTangentSpace(ray->Origin);
//	vec3 direction = TransformToTangentSpace(ray->Destination)-origin;
//
//	// get Hitpoint
//	float mult = -origin.z / direction.z;
//	vec3 hit = origin+direction*mult;
//
//	// return Result
//	if(hit.x > -Threshold && hit.x < 1+Threshold)
//	{
//		ray->Hit = true;
//		ray->HitMult = mult;
//		ray->HitCoords = vec2(hit);
//	}
//	else
//	{
//		ray->Hit = false;
//	}
//}
//
//// Deconstructor
//TraceableLine2D::~TraceableLine2D(void)
//{
//}