#include "MapData.h"
#include "BulletManager.h"

MapData::MapData(void)
{
	mapModel = new Model();

	mapModel->Mesh = MeshData::NewEmpty();
	mapModel->Material = MaterialData::FromXml("mapWire.xmf");
	//mapModel->Material = MaterialData::FromXml("floor.xmf");

	//Drawables.Add(
}

MapData::~MapData(void)
{
}

void MapData::Draw(enum DrawingPass pass)
{
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	mapModel->Draw(pass);
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}

void MapData::AddBrush(Brush* newBrush)
{
	Brushes.Add(newBrush);
	GenerateMeshFromBrushes();
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

vec3 brushPositions[8];
vec2 brushTextureCoords[8*3];

void MapData::GenerateMeshFromBrushes()
{
	MeshData::Clear();

	mapTrianglesLength = 0;

	//mapPositionDataLength = 0;
	//mapTextureDataLength = 0;
	//mapPositionIndexDataLength = 0;
	//mapTextureIndexDataLength = 0;

	Brushes.InitReader();

	vec3* tmpPos;
	vec2* tmpTex;
	vec3 tmpNor;
	vec3 positiveNor;
	vec3 curPos;
	vec3 curSize;
	vec2 curTex;
	bool assigned = false;

	while (Brushes.Read())
	{
		for (int i = 0; i < 8; i++)
		{
			curPos = Brushes.Cur->Position;
			curSize = Brushes.Cur->Size;

			if((i & 1) == 0) curPos.x += curSize.x;
			if((i & 2) == 0) curPos.y += curSize.y;
			if((i & 4) == 0) curPos.z += curSize.z;

			//brushIndices[i] = mapPositionDataLength;
			//mapPositionBuffer[mapPositionDataLength++] = curPos;

			brushPositions[i] = curPos;

			//Generate Texture Coords
			for (int j = 0; j < 3; j++)
			{
				switch (j)
				{
				case 0:
					curTex = vec2(curPos.x*0.1f,curPos.z*0.1f);
					break;
				case 1:
					curTex = vec2(curPos.x*0.1f,curPos.y*0.1f);
					break;
				case 2:
					curTex = vec2(curPos.z*0.1f,curPos.y*0.1f);
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
	// Perform Operations Per Triangle
	MapTriangle::GenerateAllSubTriangles();

	for (int i = 0; i < mapTrianglesLength; i++)
	{
		//Get Triangle Positions
		tmpPos = mapTriangles[i].Point;

		//Get Triangle Texture Coords
		tmpTex = mapTriangles[i].TexCoord;

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
}

void MapData::makeFace(int index0, int index1, int index2, int index3, int textureMapping)
{
	//mapPositionIndexBuffer[mapPositionIndexDataLength++] = brushIndices[index0];
	//mapPositionIndexBuffer[mapPositionIndexDataLength++] = brushIndices[index1];
	//mapPositionIndexBuffer[mapPositionIndexDataLength++] = brushIndices[index2];
	//mapTextureIndexBuffer[mapTextureIndexDataLength++] = TextureIndices[index0+8*textureMapping];
	//mapTextureIndexBuffer[mapTextureIndexDataLength++] = TextureIndices[index1+8*textureMapping];
	//mapTextureIndexBuffer[mapTextureIndexDataLength++] = TextureIndices[index2+8*textureMapping];

	//mapPositionIndexBuffer[mapPositionIndexDataLength++] = brushIndices[index1];
	//mapPositionIndexBuffer[mapPositionIndexDataLength++] = brushIndices[index3];
	//mapPositionIndexBuffer[mapPositionIndexDataLength++] = brushIndices[index2];
	//mapTextureIndexBuffer[mapTextureIndexDataLength++] = TextureIndices[index1+8*textureMapping];
	//mapTextureIndexBuffer[mapTextureIndexDataLength++] = TextureIndices[index3+8*textureMapping];
	//mapTextureIndexBuffer[mapTextureIndexDataLength++] = TextureIndices[index2+8*textureMapping];

	mapTriangles[mapTrianglesLength++] = MapTriangle(
		brushPositions[index0],
		brushPositions[index1],
		brushPositions[index2],
		brushTextureCoords[index0+8*textureMapping],
		brushTextureCoords[index1+8*textureMapping],
		brushTextureCoords[index2+8*textureMapping]);

	mapTriangles[mapTrianglesLength++] = MapTriangle(
		brushPositions[index1],
		brushPositions[index3],
		brushPositions[index2],
		brushTextureCoords[index1+8*textureMapping],
		brushTextureCoords[index3+8*textureMapping],
		brushTextureCoords[index2+8*textureMapping]);
}