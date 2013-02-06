#include "MapMeshData.h"

MapTriangle mapTriangles[128000];
uint mapTrianglesLength;

// Constructor
SimpleTriangle::SimpleTriangle(vec3 point1, vec3 point2, vec3 point3,	
						 vec2 texCoord1, vec2 texCoord2, vec2 texCoord3)
{
	// Set Basic Data
	Point[0] = point1;
	Point[1] = point2;
	Point[2] = point3;
	TexCoord[0] = texCoord1;
	TexCoord[1] = texCoord2;
	TexCoord[2] = texCoord3;
}

// Constructor
MapTriangle::MapTriangle(vec3 point1, vec3 point2, vec3 point3,	
						 vec2 texCoord1, vec2 texCoord2, vec2 texCoord3)
{
	// Set Basic Data
	Point[0] = point1;
	Point[1] = point2;
	Point[2] = point3;
	TexCoord[0] = texCoord1;
	TexCoord[1] = texCoord2;
	TexCoord[2] = texCoord3;

	// Initialize
	InitTransformation();
}

void MapTriangle::GenerateAllSubTriangles()
{
	for (int i = 0; i < mapTrianglesLength; i++)
	{
		mapTriangles[i].GenerateSubTriangles();
	}
}

void MapTriangle::GenerateSubTriangles()
{
	#pragma region local Variables
	vec2 newPoints[2];
	int newPointCount = 0;
	vec3 origin, destination;
	int nextIndex;
	Ray ray;
	MapTriangle curTri;
	vec2 localPos;
	bool exists;
	vec2 lines[256];
	uint lineDataCount = 0;

	float borderSubdivs[3][64];
	int borderSubdivLength[3];
	borderSubdivLength[0] = 0;
	borderSubdivLength[1] = 0;
	borderSubdivLength[2] = 0;
	#pragma endregion

	// Find intersections
	#pragma region intersections
	for (int i = 0; i < mapTrianglesLength; i++)
	{
		curTri = mapTriangles[i];
		newPointCount = 0;
		if(IntersectionHelper(curTri))
		{
			// Shoot rays at this triangle
			for (int i = 0; i < 3; i++)
			{
				// Ray Origin
				origin = curTri.Point[i];

				// Ray Destination
				nextIndex = i+1;
				if(nextIndex == 3)
					nextIndex = 0;
				destination = curTri.Point[nextIndex];

				ray.Origin = origin;
				ray.Destination = destination;

				// Parallel -> no Hit
				if(!IsParallel(&ray,0.001f))
				{
					// Ray Test
					RayTest(&ray);

					// Parse Result
					if(ray.Hit == true && ray.HitMult > 0.001f && ray.HitMult < 1.001f)
					{
						localPos = ray.HitCoords;
						exists = false;
						for (int i = 0; i < newPointCount; i++)
						{
							if(length(newPoints[i]-localPos) < 0.001f)
							{
								exists = true;
								break;
							}
						}

						if(!exists)
							newPoints[newPointCount++] = localPos;
					}
				}
			}

			// Shoot rays at cur triangle
			for (int i = 0; i < 3; i++)
			{
				// Ray Origin
				origin = Point[i];

				// Ray Destination
				nextIndex = i+1;
				if(nextIndex == 3)
					nextIndex = 0;
				destination = Point[nextIndex];

				ray.Origin = origin;
				ray.Destination = destination;

				// Parallel -> no Hit
				if(!curTri.IsParallel(&ray,0.001f))
				{
					// Ray Test
					curTri.RayTest(&ray);

					// Parse Result
					if(ray.Hit == true && ray.HitMult > 0.0f && ray.HitMult < 1.0f)
					{
						localPos = vec2(TransformToTangentSpace(ray.HitPos()));
						exists = false;
						for (int i = 0; i < newPointCount; i++)
						{
							if(length(newPoints[i]-localPos) < 0.001f)
							{
								exists = true;
								break;
							}
						}

						if(!exists)
						{
							newPoints[newPointCount++] = localPos;
							borderSubdivs[i][borderSubdivLength[i]++] = ray.HitMult;
						}
					}
				}
			}

			// Save Line
			if(newPointCount == 2)
			{
				lines[lineDataCount++] = newPoints[0];
				lines[lineDataCount++] = newPoints[1];
			}
		}
	}
	#pragma endregion

	#pragma region local Variables
	int curDataLength = 0;
	vec2 originTs, destinationTs;
	vec2 lastPoint, nextPoint;
	float* curSubDivs;
	#pragma endregion

	// Asseble sub triangles
	#pragma region assembling

	// Generate borderlines
	for (int i = 0; i < 3; i++)
	{
		// Ray Origin
		originTs = ownPointsTs[i];
		lastPoint  = originTs;

		// Ray Destination
		nextIndex = i+1;
		if(nextIndex == 3)
			nextIndex = 0;
		destinationTs = ownPointsTs[nextIndex];

		// Sort Subdivs
		curDataLength = borderSubdivLength[i];
		curSubDivs = borderSubdivs[i];
		#pragma region stupid selection sort >.<
		for (int i = 0; i < curDataLength; i++)
		{
			float a = curSubDivs[i];
			for (int j = i+1; j < curDataLength; j++)
			{
				if(curSubDivs[j] < a)
				{
					a = curSubDivs[j];
					curSubDivs[j] = curSubDivs[i];
					curSubDivs[i] = a;
				}
			}
		}
		#pragma endregion

		// Clean Mesh
		for (int i = 0; i < lineDataCount; i++)
		{
			if(length(lines[i]-originTs) < 0.001f)
				lines[i]=originTs;
			if(length(lines[i]-destinationTs) < 0.001f)
				lines[i]=destinationTs;
		}

		for (int i = 0; i < curDataLength; i++)
		{
			nextPoint = originTs + destinationTs-originTs * curSubDivs[i];

			// Create Line
			lines[lineDataCount++] = lastPoint;
			lines[lineDataCount++] = nextPoint;

			lastPoint = nextPoint;

			//// Clean Mesh
			//for (int i = 0; i < lineDataCount; i++)
			//{
			//	if(length(lines[i]-lastPoint) < 0.001f)
			//		lines[i]=lastPoint;
			//}

		}

		lines[lineDataCount++] = lastPoint;
		lines[lineDataCount++] = destinationTs;
	}
	#pragma endregion
}

bool MapTriangle::IntersectionHelper(MapTriangle tri)
{
	PlnEquation plane = PlnEquation(Point[0],Point[1],Point[2]);

	// On both sides?
	bool sideNeg = false;
	bool sidePos = false;
	bool centerOnly = true;
	float checkValue;
	for (int i = 0; i < 3; i++)
	{
		checkValue = plane.Check(tri.Point[i]);

		if(checkValue > 0.0001)
		{
			sidePos = true;
			centerOnly = false;

			if(sideNeg)
				break;
		}
		else if(checkValue < -0.0001)
		{
			sideNeg = true;
			centerOnly = false;

			if(sideNeg)
				break;
		}
		else
		{
			sidePos = true;
			sideNeg = true;
		}
	}
	if(!sideNeg || !sidePos || centerOnly)
		return false;

	// Do not collide when 2 or more points are common
	vec3 pointA, pointB;
	bool hasOneCommonPoint = false;
	for (int j = 0; j < 3; j++)
	{
		pointA = Point[j];
		for (int k = 0; k < 3; k++)
		{
			pointB = tri.Point[k];

			if(pointA == pointB)
			{
				if(hasOneCommonPoint)
					return false;

				hasOneCommonPoint = true;
			}
		}
	}
}