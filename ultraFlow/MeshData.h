#pragma once
#include "Base.h"
#include "GenFunc.h"
//#include "ListContainer.h"

const int buffersize = 128000;
extern GLfloat positionBuffer[buffersize][3];
extern GLfloat textureBuffer[buffersize][2];
extern GLfloat normalBuffer[buffersize][3];
extern GLfloat tangentBuffer[buffersize][3];
extern GLfloat biNormalBuffer[buffersize][3];
extern GLuint indexBuffer[buffersize];
extern float indexedPositionBuffer[buffersize*3];
extern float indexedtextureBuffer[buffersize*2];
extern float indexedNormalBuffer[buffersize*3];
extern btVector3 positions[buffersize];

extern GLfloat maxDistToCenter;

extern int dataLenght;
extern int dataLenghtIndex;

class MeshData
{
private:
	GLuint vbo[6],vao;
	bool hasBuffers;
public:
	int Length;
	char Name[MaxNameLen];
	float maxRadius;

	MeshData(void);
	~MeshData(void);

	static MeshData* FromObj(char* source);
	static MeshData* NewEmpty();

	void Bind(void);
	void UploadMeshData();

	static void Clear(void);
	static void GenerateTangent(void);
	static void CalcBSphere();
};
	//GLfloat **positions;
	//GLfloat **normals;
	//GLuint *indices;

extern ListContainer<MeshData> Meshes;