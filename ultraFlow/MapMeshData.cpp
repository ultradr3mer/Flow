#include "MapMeshData.h"

//ListContainer<MapTriangle> mapTriangles;

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
SimpleTriangle::SimpleTriangle(vec3 points[3], vec2 texCoords[3])
{
	// Set Basic Data
	Point[0] = points[0];
	Point[1] = points[1];
	Point[2] = points[2];
	TexCoord[0] = texCoords[0];
	TexCoord[1] = texCoords[1];
	TexCoord[2] = texCoords[2];
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

// Center Point
vec3 SimpleTriangle::Center()
{
	return (Point[0]+Point[1]+Point[2])/3.0f;
}

// Copy data
SimpleTriangle* SimpleTriangle::Copy()
{
	return new SimpleTriangle(Point,TexCoord);
}


// Normal
vec3 SimpleTriangle::Normal()
{
	return normalize(cross(Point[1]-Point[0],Point[2]-Point[0]));
}

// Set Frontside
void SimpleTriangle::SetFront(vec3 normal)
{
	vec3 curNormal = cross(Point[1]-Point[0],Point[2]-Point[0]);

	// is normal correct?
	if(dot(normal, curNormal) < 0)
	{
		// Swap point 1 and 2
		vec3 tmpPoint = Point[1];
		Point[1] = Point[2];
		Point[2] = tmpPoint;

		vec2 tmpTex = TexCoord[1];
		TexCoord[1] = TexCoord[2];
		TexCoord[2] = tmpTex;
	}
}

void MapTriangle::GenerateSubTriangles(ListContainer<MapTriangle>* intersectionTriangles)
{
	SubTriangles.ClearDelete();

	float threshold = 0.001f;

	#pragma region local Variables
	Ray lineRay;
	ListContainer<Line2d> lines;
	float lineSubdivs[64];
	int lineSubdivCount = 0;
	vec2 cutPoint;
	vec2 lastLinePoint, nextLinePoint;
	int nextIndex;

	float borderSubdivs[3][64];
	int borderSubdivLength[3];
	borderSubdivLength[0] = 0;
	borderSubdivLength[1] = 0;
	borderSubdivLength[2] = 0;
	#pragma endregion

	// Find intersections
	#pragma region intersections
	intersectionTriangles->InitReader();
	while (intersectionTriangles->Read())
	{
		Ray intersectionLine = GetIntersectionLine(intersectionTriangles->Cur);
		intersectionLine.Origin = TransformToTangentSpace(intersectionLine.Origin);
		intersectionLine.Destination = TransformToTangentSpace(intersectionLine.Destination);

		// Save Line
		if(intersectionLine.Hit && length(intersectionLine.Origin-intersectionLine.Destination) > threshold)
		{
			lines.Add(new Line2d(vec2(intersectionLine.Origin),vec2(intersectionLine.Destination)));
		}
	}
	#pragma endregion

	#pragma region local Variables
	bool removedLine = true;
	int linesLength;
	vec2 lineA[2], lineB[2];
	int lineADir, lineBDir;
	PlnEquation linePln;
	bool canBeConnected;
	float checkResult;
	Line2d* curLine;
	#pragma endregion

	#pragma region line cleanup
	// Connect lines in a row
	while(removedLine && lines.Length > 1)
	{
		removedLine = false;
		lines.GetIndex();
		linesLength = lines.Length;
		for (int i = 1; i < linesLength; i++)
		{
			lineA[0] = lines.Index[i]->Origin;
			lineA[1] = lines.Index[i]->Destination;

			for (int j = i-1; j >= 0; j--)
			{
				if(lines.Index[j] != nullptr)
				{
					lineB[0] = lines.Index[j]->Origin;
					lineB[1] = lines.Index[j]->Destination;
					for (int k = 0; k < 4; k++)
					{
						lineADir = (k & 1) == 0 ? 0:1;
						lineBDir = (k & 2) == 0 ? 0:1;

						canBeConnected = (length(lineA[lineADir]-lineB[lineBDir])<threshold);

						// check if both go in the same direction
						if(canBeConnected)
						{
							if(dot(
								normalize(lineA[lineADir]-lineA[1-lineADir]),
								normalize(lineB[1-lineBDir]-lineB[lineBDir])) 
								< 1 -threshold)
								canBeConnected = false;
						}

						if(canBeConnected)
						{
							checkResult = lines.Index[i]->Pln.Check(vec3(lineB[1-lineBDir],0.0f));
							if(checkResult > threshold || checkResult < -threshold)
								canBeConnected = false;
						}

						if(canBeConnected)
						{
							lines.Index[i]->Origin = lineA[1-lineADir];
							lines.Index[i]->Destination = lineB[1-lineBDir];

							lines.RemoveDelete(lines.Index[j]);
							lines.Index[j] = nullptr;

							removedLine = true;
						}
					}
				}
			}
		}
	}

	//Split Crossing lines
	for (int i = 0; i < lines.Length; i++)
	{
		lines.GetIndex();
		curLine = lines.Index[i];
		curLine->Threshold = -0.01f;
		for (int j = i-1; j >= 0; j--)
		{
			if(curLine->Intersect(lines.Index[j]))
			{
				//Generate new lines / modyfy old ones
				if(length(curLine->intersectionPoint-curLine->Destination) > threshold && 
					length(curLine->Origin-curLine->intersectionPoint) > threshold)
				{
					lines.Add(new Line2d(curLine->intersectionPoint,curLine->Destination));
					curLine->Destination = curLine->intersectionPoint;
				}
				if(length(curLine->intersectionPoint-lines.Index[j]->Destination) > threshold &&
					length(lines.Index[j]->Origin-curLine->intersectionPoint) > threshold)
				{
					lines.Add(new Line2d(curLine->intersectionPoint,lines.Index[j]->Destination));
					lines.Index[j]->Destination = curLine->intersectionPoint;
				}

				////Modify old ones
				//curLine->Destination = curLine->intersectionPoint;
				//lines.Index[j]->Destination = curLine->intersectionPoint;
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

	// Generate borderlines
	#pragma region borders

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
		for (int j = 0; j < curDataLength; j++)
		{
			float a = curSubDivs[j];
			for (int k = j+1; k < curDataLength; k++)
			{
				if(curSubDivs[k] < a)
				{
					a = curSubDivs[k];
					curSubDivs[k] = curSubDivs[j];
					curSubDivs[j] = a;
				}
			}
		}
		#pragma endregion

		for (int j = 0; j < curDataLength; j++)
		{
			nextPoint = destinationTs * (1-curSubDivs[j]) + originTs * curSubDivs[j];

			// Create Line
			lines.Add(new Line2d(lastPoint,nextPoint));
			//lines[lineDataCount++] = lastPoint;
			//lines[lineDataCount++] = nextPoint;

			lastPoint = nextPoint;

		}

		lines.Add(new Line2d(lastPoint,destinationTs));
		//lines[lineDataCount++] = lastPoint;
		//lines[lineDataCount++] = destinationTs;
	}
	#pragma endregion

	#pragma region local Variables
	vec2 uniquePoints[256];
	int uniquePointCount = 0;
	bool pointExists = false;
	vec2 currentTriangle[3];
	TraceableTriangle* currentTrTriangle;
	bool triangleIsValid;
	vec3 currentTriangleWorldCoords[3];
	vec2 currentTriangleTextureCorrds[3];
	vec3 borderVectorsWorld[2];
	vec2 borderVectorsTexture[2];
	vec3 vecA,vecB;
	vec2 vecTexA,vecTexB;
	bool hasThisPoint;
	bool isDuplicate;
	vec3 curPoint;
	vec3 helpVec;
	vec2 helpVecTex;
	ListContainer<TraceableTriangle> trSubTriangles;
	#pragma endregion

	// Asseble sub triangles
	#pragma region assembling
	lineRay = Ray();

	// Find unique points
	lines.InitReader();
	while(lines.Read())
	//for (int i = 0; i < lineDataCount; i++)
	{
		pointExists = false;
		for (int j = 0; j < uniquePointCount; j++)
		{
			if(length(uniquePoints[j]-lines.Cur->Origin)<threshold)
			{
				pointExists = true;
				break;
			}
		}
		if(!pointExists)
		{
			uniquePoints[uniquePointCount++] = lines.Cur->Origin;
		}

		pointExists = false;
		for (int j = 0; j < uniquePointCount; j++)
		{
			if(length(uniquePoints[j]-lines.Cur->Destination)<threshold)
			{
				pointExists = true;
				break;
			}
		}
		if(!pointExists)
		{
			uniquePoints[uniquePointCount++] = lines.Cur->Destination;
		}
	}

	// generate all possible triangles
	lines.GetIndex();
	for (int i = 0; i < lines.Length; i++)
	{
		// Get first 2 Points from Current line Line
		currentTriangle[0] = lines.Index[i]->Origin;
		currentTriangle[1] = lines.Index[i]->Destination;

		for (int j = 0; j < uniquePointCount; j++)
		{
			// Pick any Point as third one
			currentTriangle[2] = uniquePoints[j];

			// Create Tracable Triangle
			currentTrTriangle = new TraceableTriangle(
				vec3(currentTriangle[0],0.0f),
				vec3(currentTriangle[1],0.0f),
				vec3(currentTriangle[2],0.0f));

			//currentTrTriangle->Threshold *= -1;

			// Ceck if Current point is not part of current line
			triangleIsValid = (length(uniquePoints[j]-currentTriangle[0])>threshold &&
				length(uniquePoints[j]-currentTriangle[1])>threshold);


			if(triangleIsValid)
			{
				// Check if surface content big enougth
				if( length(cross(
					currentTrTriangle->Point[1]-currentTrTriangle->Point[0],
					currentTrTriangle->Point[2]-currentTrTriangle->Point[0]))
					< threshold)
					triangleIsValid = false;
			}

			// Check if Triangle Intersects with a line
			if(triangleIsValid)
			{
				currentTrTriangle->Threshold = -0.0025f;
				lineRay.Threshold  = 0;

				for (int k = 0; k < lines.Length; k++)
				{
					vec2 first = lines.Index[k]->Origin;
					vec2 second = lines.Index[k]->Destination;
					vec2 dir = normalize(first-second)*Threshold;
					lineRay.Origin = vec3(first-dir,0.0f);
					lineRay.Destination = vec3(second+dir,0.0f);					
					currentTrTriangle->PlaneInternTesting(&lineRay);
					
					if(lineRay.Hit)
					{
						triangleIsValid = false;
						break;
					}
				}
				currentTrTriangle->Threshold = 0.001f;
			}

			// Is this Triangle inside of another triangle or another triangle inside this?
			if(triangleIsValid)
			{
				trSubTriangles.InitReader();
				while(trSubTriangles.Read())
				{
					if(currentTrTriangle->PointTest(trSubTriangles.Cur->Center) ||
						trSubTriangles.Cur->PointTest(currentTrTriangle->Center))
					{
						triangleIsValid = false;
						break;
					}
				}
			}

			// Save Triangle ?
			if(triangleIsValid)
			{
				vecA = Point[1]-Point[0];
				vecB = Point[2]-Point[0];

				vecTexA = TexCoord[1]-TexCoord[0];
				vecTexB = TexCoord[2]-TexCoord[0];

				// Generate World and Texture Coords
				for (int k = 0; k < 3; k++)
				{
					currentTriangleWorldCoords[k] = Point[0]+vecA*currentTriangle[k].x+vecB*currentTriangle[k].y;
					currentTriangleTextureCorrds[k] = TexCoord[0]+vecTexA*currentTriangle[k].x+vecTexB*currentTriangle[k].y;
				}

				// is Clockwise?
				if(dot(cross(vecA,vecB),
					cross(currentTriangleWorldCoords[1]-currentTriangleWorldCoords[0],
					currentTriangleWorldCoords[2]-currentTriangleWorldCoords[0])) < 0)
				{
					helpVec = currentTriangleWorldCoords[0];
					currentTriangleWorldCoords[0] = currentTriangleWorldCoords[1];
					currentTriangleWorldCoords[1] = helpVec;

					helpVecTex = currentTriangleTextureCorrds[0];
					currentTriangleTextureCorrds[0] = currentTriangleTextureCorrds[1];
					currentTriangleTextureCorrds[1] = helpVecTex;
				}


				//// Save new Lines

				lines.Add(new Line2d(currentTriangle[1],currentTriangle[2]));
				lines.Add(new Line2d(currentTriangle[2],currentTriangle[0]));

				//if(lineDataCount >= 252)
				//	printf("Error Generating Map Mesh");

				//lines[lineDataCount++] = currentTriangle[1];
				//lines[lineDataCount++] = currentTriangle[2];

				//lines[lineDataCount++] = currentTriangle[2];
				//lines[lineDataCount++] = currentTriangle[0];

				lines.GetIndex();

				// Save Tracable Triangle
				trSubTriangles.Add(currentTrTriangle);

				// Save Sub Triangle
				SubTriangles.Add(new SimpleTriangle(
					currentTriangleWorldCoords,
					currentTriangleTextureCorrds));
			}
			else
			{
				delete currentTrTriangle;
			}
		}
	}

	#pragma endregion
}

bool MapTriangle::IntersectionHelper(MapTriangle* tri)
{
	PlnEquation plane = PlnEquation(Point[0],Point[1],Point[2]);

	// On both sides?
	bool sideNeg = false;
	bool sidePos = false;
	bool centerOnly = true;
	float checkValue;
	for (int i = 0; i < 3; i++)
	{
		checkValue = plane.Check(tri->Point[i]);

		if(checkValue > 0.001)
		{
			sidePos = true;
			centerOnly = false;

			if(sideNeg)
				break;
		}
		else if(checkValue < -0.001)
		{
			sideNeg = true;
			centerOnly = false;

			if(sidePos)
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
			pointB = tri->Point[k];

			if(pointA == pointB)
			{
				if(hasOneCommonPoint)
					return false;

				hasOneCommonPoint = true;
			}
		}
	}

	return true;
}