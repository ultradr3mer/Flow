#include "MapData.h"
#include "BulletManager.h"

MapData::MapData(void)
{
	mapModel = new Model();

	mapModel->Mesh = MeshData::NewEmpty();
	//mapModel->Material = MaterialData::FromXml("mapWire.xmf");
	mapModel->Material = MaterialData::FromXml("floor.xmf");

	//Drawables.Add(
}

MapData::~MapData(void)
{
}

void MapData::Draw(enum DrawingPass pass)
{
	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	//mapModel->Draw(pass);
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	mapModel->Draw(pass);
}

void MapData::AddBrush(Brush* newBrush)
{
	Brushes.Add(newBrush);
	GenerateMeshFromBrushes(newBrush);
	MeshData::GenerateTangent();
	MeshData::CalcBSphere();
	mapModel->Mesh->UploadMeshData();
	updateCollisionModel();
}

int vertStride = sizeof(btVector3);
int indexStride = 3*sizeof(int);
int gIndices[buffersize*3];
btVector3 gVertices[buffersize];

void MapData::updateCollisionModel()
{
	//btVector3* gVertices = new btVector3[dataLenght];

	for (int i = 0; i < dataLenght; i++)
	{
		gVertices[i] = btVector3(
			positionBuffer[i][0],
			positionBuffer[i][1],
			positionBuffer[i][2]);
	}
	for (int i = 0; i < dataLenghtIndex; i++)
	{
		gIndices[i] = indexBuffer[i];
	}

	btTriangleIndexVertexArray* m_indexVertexArrays = new btTriangleIndexVertexArray(dataLenghtIndex/3,
	gIndices,
	indexStride,
	dataLenght,(btScalar*) &gVertices[0].x(),vertStride);

	btCollisionShape* groundShape = new btBvhTriangleMeshShape(m_indexVertexArrays,true);
	
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)));
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
    btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
    dynamicsWorld->addRigidBody(groundRigidBody);
}

//vec3 mapPositionBuffer[buffersize];
//uint mapPositionDataLength;
//uint mapPositionIndexBuffer[buffersize];
//uint mapPositionIndexDataLength;
//
//vec2 mapTextureBuffer[buffersize];
//uint mapTextureDataLength;
//uint mapTextureIndexBuffer[buffersize];
//uint mapTextureIndexDataLength;

//uint brushIndices[8];
//uint TextureIndices[8*3];

//vec3 brushPositions[8];
//vec2 brushTextureCoords[8*3];

void MapData::GenerateMeshFromBrushes(Brush* newBrush)
{
	float threshold = 0.001f;
	MeshData::Clear();

	//mapTriangles.ClearDelete();

	//mapPositionDataLength = 0;
	//mapTextureDataLength = 0;
	//mapPositionIndexDataLength = 0;
	//mapTextureIndexDataLength = 0;

	vec3* tmpPos;
	vec2* tmpTex;
	vec3 tmpNor;
	vec3 positiveNor;
	vec3 curPos;
	vec3 curSize;
	vec2 curTex;
	bool assigned = false;
	ListContainer<SimpleTriangle>* subTriangles;
	ListContainer<SimpleTriangle> allSubTriangles;
	ListContainer<Brush>* intersectingBrushes;
	bool isHidden = false;

	//allSubTriangles.PerformCleanup = false;

	// Prepare intersections on new Brush
	newBrush->GenerateTriangles();
	newBrush->PrepareIntersections(&Brushes);
	intersectingBrushes = &newBrush->IntersectingBrushes;

	// Prepare intersections on new Brushes Intersections
	intersectingBrushes->GetIndex();
	for (int i = 0; i < intersectingBrushes->Length; i++)
	{
		intersectingBrushes->Index[i]->PrepareIntersections(&Brushes);
	}

	//Brushes.GetIndex();
	//for (int i = 0; i < Brushes.Length; i++)
	//{
	//	Brushes.Index[i]->GenerateTriangles();
	//	Brushes.Index[i]->PrepareIntersections(&Brushes);
	//}

	Brushes.GetIndex();
	for (int i = 0; i < Brushes.Length; i++)
	{
		Brushes.Index[i]->PerformIntersections(&allSubTriangles);
	}

	#pragma region clean up
	//Triangle merging / clean up
	int allSubTrianglesLength;
	bool triangleRemoved = true;
	SimpleTriangle *triA, *triB;
	vec3 curPoint;
	int sharedPointCount;
	int triAIndices[3], triBIndices[3];
	bool canBeMerged;
	vec3 uniquePointTsCoords;
	TraceableTriangle curTri;
	vec3 oldNormal;
	bool isDuplicate;
	bool canBeRotated;
	// Continue as long as last run could merge 2 tris
	while(triangleRemoved)
	{
		triangleRemoved = false;
		allSubTriangles.GetIndex();
		allSubTrianglesLength = allSubTriangles.Length;

		// Get first Triangle
		for (int i = 0; i < allSubTrianglesLength; i++)
		{
			triA = allSubTriangles.Index[i];

			// Grab another Triangle
			for (int j = i-1; j >= 0; j--)
			{
				isDuplicate = false;

				// Does Triangle still exist?
				canBeMerged = (allSubTriangles.Index[j] != nullptr);

				// Share same normal ?
				if(canBeMerged)
				{
					triB = allSubTriangles.Index[j];
					if(dot(triA->Normal(),triB->Normal()) < 1-threshold)
						canBeMerged = false;
				}
				
				// Check if triangles share 2 Points
				if(canBeMerged)
				{
					sharedPointCount = 0;
					for (int k = 0; k < 3; k++)
					{
						for (int l = 0; l < 3; l++)
						{
							if(length(triA->Point[k]-triB->Point[l]) < threshold * 10.0f)
							{
								sharedPointCount++;

								// Sharing 3 points means B will be removed
								if(sharedPointCount > 2)
								{
									isDuplicate = true;
									break;
								}

								// Save indices for later use
								triAIndices[sharedPointCount] = k;
								triBIndices[sharedPointCount] = l;
								break;
							}
						}
					}
					if(sharedPointCount < 2)
						canBeMerged = false;
				}

				// Remove Triangle B
				if(isDuplicate)
				{
					allSubTriangles.Remove(triB);
					allSubTriangles.Index[j] = nullptr;

					triangleRemoved = true;
					canBeMerged = false;
				}

				// Translate to tangent space
				if(canBeMerged)
				{
					// Get unused Indices
					triAIndices[0] = 0;
					for (int j = 0; j < 3; j++)
					{
						if(triAIndices[(j&1)+1] == triAIndices[0])
							triAIndices[0]++;
					}
					triBIndices[0] = 0;
					for (int j = 0; j < 3; j++)
					{
						if(triBIndices[(j&1)+1] == triBIndices[0])
							triBIndices[0]++;
					}

					// Generate tracable triangle (Point 0 is unique, 1 and 2 are shared)
					curTri = TraceableTriangle(
						triA->Point[triAIndices[0]],
						triA->Point[triAIndices[1]],
						triA->Point[triAIndices[2]]);

					// Translate the unique point of triangle b into tangent space
					uniquePointTsCoords = curTri.TransformToTangentSpace(
						triB->Point[triBIndices[0]]);

					// Check if Point is in the plane of triA
					if(uniquePointTsCoords.z < -threshold ||
						uniquePointTsCoords.z > threshold) 
						canBeMerged = false;
				}

				canBeRotated = canBeMerged;

				// Check if one of the shared points is between the unique points
				if(canBeMerged)
				{
					// Is in line with Point 0 - Point 1
					if(uniquePointTsCoords.y > -threshold &&
						uniquePointTsCoords.y < threshold &&
						uniquePointTsCoords.x > 1.0f) 
					{
						// Extend Triangle A
						oldNormal = triA->Normal();
						triA->Point[triAIndices[1]] = triB->Point[triBIndices[0]];
						triA->TexCoord[triAIndices[1]] = triB->TexCoord[triBIndices[0]];
						triA->SetFront(oldNormal);

						// Remove Triangle B
						allSubTriangles.Remove(triB);
						allSubTriangles.Index[j] = nullptr;

						triangleRemoved = true;
						canBeMerged = false;
						canBeRotated = false;
					}
				}

				if(canBeMerged)
				{
					// Is in line with Point 0 - Point 2
					if(uniquePointTsCoords.x > -threshold &&
						uniquePointTsCoords.x < threshold &&
						uniquePointTsCoords.y > 1.0f) 
					{
						// Extend triangle A
						oldNormal = triA->Normal();
						triA->Point[triAIndices[2]] = triB->Point[triBIndices[0]];
						triA->TexCoord[triAIndices[2]] = triB->TexCoord[triBIndices[0]];
						triA->SetFront(oldNormal);

						// Remove triangle B
						allSubTriangles.Remove(triB);
						allSubTriangles.Index[j] = nullptr;

						triangleRemoved = true;
						canBeRotated = false;
					}
				}

				// Check if A and B form a convex shape
				if(canBeRotated)
				{
					if(uniquePointTsCoords.y < threshold ||
						uniquePointTsCoords.x < threshold)
						canBeRotated = false;
				}

				// Rotate to find other merging possibilities
				if(canBeRotated)
				{
					//oldNormal = triA->Normal();

					// Rotate triangle A
					triA->Point[triAIndices[2]] = triA->Point[triAIndices[0]];
					triA->Point[triAIndices[0]] = triA->Point[triAIndices[1]];
					triA->Point[triAIndices[1]] = triB->Point[triBIndices[0]];
					triA->TexCoord[triAIndices[2]] = triA->TexCoord[triAIndices[0]];
					triA->TexCoord[triAIndices[0]] = triA->TexCoord[triAIndices[1]];
					triA->TexCoord[triAIndices[1]] = triB->TexCoord[triBIndices[0]];
					
					// Rotate triangle B
					triB->Point[triBIndices[0]] = triB->Point[triBIndices[2]];
					triB->Point[triBIndices[1]] = triA->Point[triAIndices[1]];
					triB->Point[triBIndices[2]] = triA->Point[triAIndices[2]];
					triB->TexCoord[triBIndices[0]] = triB->TexCoord[triBIndices[2]];
					triB->TexCoord[triBIndices[1]] = triA->TexCoord[triAIndices[1]];
					triB->TexCoord[triBIndices[2]] = triA->TexCoord[triAIndices[2]];

					//// Recalculate outside
					//triA->SetFront(oldNormal);
					//triB->SetFront(oldNormal);
				}
			}
		}
	}
	#pragma endregion

	allSubTriangles.InitReader();
	while(allSubTriangles.Read())
	{
		//Get Triangle Positions
		tmpPos = allSubTriangles.Cur->Point;

		//Get Triangle Texture Coords
		tmpTex = allSubTriangles.Cur->TexCoord;

		tmpNor = normalize(cross(tmpPos[1]-tmpPos[0],tmpPos[2]-tmpPos[0]));

		positiveNor = tmpNor;
		positiveNor.x *= sign(positiveNor.x);
		positiveNor.y *= sign(positiveNor.y);
		positiveNor.z *= sign(positiveNor.z);

		for (int j = 0; j < 3; j++)
		{
			assigned = false;

			// check if simmilar vertex exists and assign corresponding index
			for (int k = 0; k < dataLenght; k++)
			{
				if(//check if pos is the same
					tmpPos[j].x == positionBuffer[k][0] &&
					tmpPos[j].y == positionBuffer[k][1] &&
					tmpPos[j].z == positionBuffer[k][2] &&
					//check if texture is the same
					tmpTex[j].x == textureBuffer[k][0] &&
					tmpTex[j].y == textureBuffer[k][1] &&
					//check if normal is the same
					tmpNor.x == normalBuffer[k][0] &&
					tmpNor.y == normalBuffer[k][1] &&
					tmpNor.z == normalBuffer[k][2])
				{
					indexBuffer[dataLenghtIndex++] = k;
					assigned = true;
					break;
				}
			}

			// if there is no simmilar vertex create new one
			if(!assigned)
			{
				//save position data
				positionBuffer[dataLenght][0] = tmpPos[j].x;
				positionBuffer[dataLenght][1] = tmpPos[j].y;
				positionBuffer[dataLenght][2] = tmpPos[j].z;
				//save texture data
				textureBuffer[dataLenght][0] = tmpTex[j].x;
				textureBuffer[dataLenght][1] = tmpTex[j].y;
				//save normal
				normalBuffer[dataLenght][0] = tmpNor.x;
				normalBuffer[dataLenght][1] = tmpNor.y;
				normalBuffer[dataLenght][2] = tmpNor.z;

				//save index
				indexBuffer[dataLenghtIndex++] = dataLenght++;
			}
		}	
	}


	//for (int i = 0; i < Brushes.Length; i++)
	//{
	//	Brushes.Index[i]->CleanupIntersections();
	//}
}
