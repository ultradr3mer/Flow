#include "MeshData.h"
#include "ObjLoader.h"

GLfloat positionBuffer[buffersize][3];
GLfloat textureBuffer[buffersize][2];
GLfloat normalBuffer[buffersize][3];
GLfloat tangentBuffer[buffersize][3];
GLfloat biNormalBuffer[buffersize][3];
uint indexBuffer[buffersize];
float indexedPositionBuffer[buffersize*3];
float indexedtextureBuffer[buffersize*2];
float indexedNormalBuffer[buffersize*3];
btVector3 positions[buffersize];

GLfloat maxDistToCenter;

int dataLenght = 0;
int dataLenghtIndex = 0;

//MeshData* MeshCache[4096];
//char* MeshNames[4096];
int MeshCachePosition;

ListContainer<MeshData> Meshes;// = ListContainer();

MeshData::MeshData(void)
{
	hasBuffers = false;
	UploadMeshData();

	Meshes.Add(this);
}

void MeshData::UploadMeshData()
{
	updateError("MeshData ENTER Upload");
	//GLchar *vertexsource, *fragmentsource;

	if(!hasBuffers)
	{
		/* Allocate and assign a Vertex Array Object to our handle */
		if(useVertexArrays)
		{
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);
		}
 
		// generate buffers
		glGenBuffers(6, vbo);

		hasBuffers = true;
	}

	// Position data
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, dataLenght * 3 * sizeof(GLfloat), positionBuffer, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

	// Normal data
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, dataLenght * 3 * sizeof(GLfloat), normalBuffer, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

	// Texture data
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, dataLenght * 2 * sizeof(GLfloat), textureBuffer, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

	// Tangent data
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, dataLenght * 3 * sizeof(GLfloat), tangentBuffer, GL_STATIC_DRAW);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(3);

	// BiNormal data
	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glBufferData(GL_ARRAY_BUFFER, dataLenght * 3 * sizeof(GLfloat), biNormalBuffer, GL_STATIC_DRAW);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(4);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[5]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataLenghtIndex * sizeof(GLuint), indexBuffer, GL_STATIC_DRAW);

	Length = dataLenghtIndex;
	maxRadius = maxDistToCenter;

	updateError("MeshData Upload");
}

void MeshData::Clear()
{
	dataLenght = 0;
	dataLenghtIndex = 0;
}

MeshData* MeshData::FromObj(char* source)
{
	Meshes.InitReader();
	while (Meshes.Read())
	{
		if(strcmp(source,Meshes.Cur->Name) == 0)
			return Meshes.Cur;
	}
	ObjLoader::Load(source);
	CalcBSphere();
	GenerateTangent();
	MeshData* Mesh = new MeshData();
	strcpy(Mesh->Name,source);

	return Mesh;
}

MeshData* MeshData::NewEmpty()
{
	Clear();
	CalcBSphere();
	return new MeshData();
}

MeshData::~MeshData(void)
{
	glDeleteBuffers(6, vbo);
	glDeleteVertexArrays(1, &vao);
	Meshes.Remove(this);
	//glDisableVertexAttribArray(0);
    //glDisableVertexAttribArray(1);
}

void MeshData::Bind(void)
{
	if(useVertexArrays)
	{
		glBindVertexArray(vao);
	}
	else
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(3);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(4);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[5]);
	}
}

void MeshData::GenerateTangent(void)
{
	// Info about current triangle
	vec3 vPos[3];
	vec2 vTex[3];
	int vIndices[3];
	int CurIndex;

	// Vectors of 2 triangle sides in local and texture space
	vec3 localA, localB;
	vec2 texA, texB;

	// Vector multiplicators
	float rTan, sTan;
	float rBi, sBi;

	// Result vectors
	vec3 Tangent, BiNormal;

	//Clean Up
	for (int i = 0; i < dataLenght; i++)
	{
		tangentBuffer[i][0] = 0.0f;
		tangentBuffer[i][1] = 0.0f;
		tangentBuffer[i][2] = 0.0f;

		biNormalBuffer[i][0] = 0.0f;
		biNormalBuffer[i][1] = 0.0f;
		biNormalBuffer[i][2] = 0.0f;
	}

	//Generation
	for (int i = 0; i * 3 < dataLenghtIndex; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			CurIndex = indexBuffer[i*3+j];
			vIndices[j] = CurIndex;

			vPos[j] = vec3(
				positionBuffer[CurIndex][0],
				positionBuffer[CurIndex][1],
				positionBuffer[CurIndex][2]);

			vTex[j] = vec2(
				textureBuffer[CurIndex][0],
				textureBuffer[CurIndex][1]);
		}

		// Calculate local space Vectors
		localA = vPos[1] - vPos[0];
		localB = vPos[2] - vPos[0];

		// Calculate world space Vectors
		texA = vTex[1] - vTex[0];
		texB = vTex[2] - vTex[0];

		// Calculate Multiplicators
		// For tangent
		sTan = 1.0f / (texB.x - texA.x * texB.y / texA.y);
		rTan = 1.0f / (texA.x - texB.x * texA.y / texB.y);
		// For binormal
		sBi = 1.0f / (texB.y - texA.y * texB.x / texA.x);
		rBi = 1.0f / (texA.y - texB.y * texA.x / texB.x);

		// Calculate tangent and save to buffer
		Tangent = normalize(rTan * localA + sTan * localB);
		for (int j = 0; j < 3; j++)
		{
			tangentBuffer[vIndices[j]][0] += Tangent.x;
			tangentBuffer[vIndices[j]][1] += Tangent.y;
			tangentBuffer[vIndices[j]][2] += Tangent.z;
		}

		// Calculate binormal and save to buffer
		BiNormal = normalize(rBi * localA + sBi * localB);
		for (int j = 0; j < 3; j++)
		{
			biNormalBuffer[vIndices[j]][0] += BiNormal.x;
			biNormalBuffer[vIndices[j]][1] += BiNormal.y;
			biNormalBuffer[vIndices[j]][2] += BiNormal.z;
		}
	}
	// Normalizing Result
	vec3 TmpVec;
	for (int i = 0; i < dataLenght; i++)
	{
		//Normalize Tangent
		TmpVec =vec3(
			tangentBuffer[i][0],
			tangentBuffer[i][1],
			tangentBuffer[i][2]);

		TmpVec = normalize(TmpVec);

		tangentBuffer[i][0] = TmpVec.x;
		tangentBuffer[i][1] = TmpVec.y;
		tangentBuffer[i][2] = TmpVec.z;

		//Normalize BiNormal
		TmpVec =vec3(
			biNormalBuffer[i][0],
			biNormalBuffer[i][1],
			biNormalBuffer[i][2]);

		TmpVec = normalize(TmpVec);

		biNormalBuffer[i][0] = TmpVec.x;
		biNormalBuffer[i][1] = TmpVec.y;
		biNormalBuffer[i][2] = TmpVec.z;
	}
}

void MeshData::CalcBSphere()
{
	maxDistToCenter = 0;
	for (int i = 0; i < dataLenght; i++)
	{
		GLfloat distToCenter = length(vec3(
			positionBuffer[i][0],
			positionBuffer[i][1],
			positionBuffer[i][2]));

		if(maxDistToCenter < distToCenter) maxDistToCenter = distToCenter;
	}
}