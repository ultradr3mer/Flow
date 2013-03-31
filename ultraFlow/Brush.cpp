#include "Brush.h"

Brush::Brush(vec3 position, vec3 size, uint texture, bool substract)
{
	Position = position;
	Size = size;
	Texture = texture;
	Substract = substract;
	Center = Position + Size * 0.5f;
	TestingSize = length(Size * 0.5f);
	IntersectingBrushes.PerformCleanup = false;
};

// Test if a Point is inside of the brush Volume
bool Brush::PointTest(vec3 Point)
{
	float threshold = 0.005f;
	return (Point.x > Position.x - threshold&& Point.x < Position.x + Size.x + threshold&&
		Point.y > Position.y - threshold&& Point.y < Position.y + Size.y + threshold&&
		Point.z > Position.z - threshold&& Point.z < Position.z + Size.z + threshold);
};

vec3 brushPositions[8];
vec2 brushTextureCoords[8*3];

// Generate Triangles
void Brush::GenerateTriangles()
{
	Triangles.ClearDelete();

	vec2 curTex;
	vec3 curPosition;
	float textureSize = 0.2f;

	for (int i = 0; i < 8; i++)
		{
			curPosition = Position;
			if((i & 1) == 0) curPosition.x += Size.x;
			if((i & 2) == 0) curPosition.y += Size.y;
			if((i & 4) == 0) curPosition.z += Size.z;

			//brushIndices[i] = mapPositionDataLength;
			//mapPositionBuffer[mapPositionDataLength++] = curPos;

			brushPositions[i] = curPosition;

			//Generate Texture Coords
			for (int j = 0; j < 3; j++)
			{
				switch (j)
				{
				case 0:
					curTex = vec2(curPosition.x*textureSize,curPosition.z*textureSize);
					break;
				case 1:
					curTex = vec2(curPosition.x*textureSize,curPosition.y*textureSize);
					break;
				case 2:
					curTex = vec2(curPosition.z*textureSize,curPosition.y*textureSize);
					break;
				}

				//TextureIndices[i+j*8] = mapTextureDataLength;
				//mapTextureBuffer[mapTextureDataLength++] = curTex;

				brushTextureCoords[i+j*8] = curTex;
			}
		}

		//Tris
		makeFace(0,1,2,3,1);
		makeFace(5,4,7,6,1);

		makeFace(1,0,5,4,0);
		makeFace(2,3,6,7,0);

		makeFace(0,2,4,6,2);
		makeFace(3,1,7,5,2);
}

void Brush::makeFace(int index0, int index1, int index2, int index3, int textureMapping)
{
	Triangles.Add(new MapTriangle(
		brushPositions[index0],
		brushPositions[index1],
		brushPositions[index2],
		brushTextureCoords[index0+8*textureMapping],
		brushTextureCoords[index1+8*textureMapping],
		brushTextureCoords[index2+8*textureMapping]));

	Triangles.Add(new MapTriangle(
		brushPositions[index1],
		brushPositions[index3],
		brushPositions[index2],
		brushTextureCoords[index1+8*textureMapping],
		brushTextureCoords[index3+8*textureMapping],
		brushTextureCoords[index2+8*textureMapping]));
}

// Generate a List for Precise Intersection Testing
void Brush::PrepareIntersections(ListContainer<Brush>* brushes)
{
	PerformedIntersections = false;
	brushes->GetIndex();
	for (int i = 0; i < brushes->Length; i++)
	{
		Brush* cur = brushes->Index[i];
		if(cur != this && length(Center-cur->Center)<(TestingSize+cur->TestingSize))
		{
			
			if(!IntersectingBrushes.Contains(cur))
				IntersectingBrushes.Add(cur);

			if(!cur->IntersectingBrushes.Contains(this))
				cur->IntersectingBrushes.Add(this);
		}
	}
}

// Generate Subtriangles for each Face
void Brush::PerformIntersections(ListContainer<SimpleTriangle>* resultingTriangles)
{
	ListContainer<SimpleTriangle>* subTriangles;

	if(!PerformedIntersections)
	{
		ListContainer<MapTriangle> IntersectionTriangles;
		IntersectionTriangles.PerformCleanup = false;
		ListContainer<MapTriangle>* curTriangles;
		bool isHidden;
		vec3 curCenter;
		float threshold = 0.01f;

		// Make a Local list with possible intersections
		IntersectingBrushes.InitReader();
		while(IntersectingBrushes.Read())
		{
			curTriangles = &IntersectingBrushes.Cur->Triangles;
			curTriangles->InitReader();
			while(curTriangles->Read())
			{
				IntersectionTriangles.Add(curTriangles->Cur);
			}
		}

		// Perform Operations Per Triangle
		Triangles.GetIndex();
		for (int i = 0; i < Triangles.Length; i++)
		{
			Triangles.Index[i]->GenerateSubTriangles(&IntersectionTriangles);

			// Perform Operations Per Sub Triangle
			subTriangles = &Triangles.Index[i]->SubTriangles;
			subTriangles->GetIndex();
			int currentLength = subTriangles->Length;
			for (int j = 0; j < currentLength; j++)
			{
				//Visibility Testing
				isHidden = false;
				curCenter = subTriangles->Index[j]->Center();
				curCenter += subTriangles->Index[j]->Normal()*threshold;
				IntersectingBrushes.InitReader();
				while (IntersectingBrushes.Read())
				{
					if(IntersectingBrushes.Cur->PointTest(curCenter))
					{
						subTriangles->RemoveDelete(subTriangles->Index[j]);
						break;
					}
				}
			}
		}
		PerformedIntersections = true;
	}

	// Return Subtriangles
	Triangles.InitReader();
	while(Triangles.Read())
	{
		subTriangles = &Triangles.Cur->SubTriangles;
		subTriangles->InitReader();
		while(subTriangles->Read())
		{
			resultingTriangles->Add(subTriangles->Cur->Copy());
		}
	}
}

// Cleanup
void Brush::CleanupIntersections()
{
	Triangles.ClearDelete();
	IntersectingBrushes.Clear();
}