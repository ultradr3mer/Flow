#include "Base.h"
#include "GenFunc.h"
//#include "ListContainer.h"

const int buffersize =128000;
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

#pragma once
class MeshData
{
public:
	MeshData(void);
	~MeshData(void);
	void Bind(void);
	static MeshData* FromObj(char* source);
	static void Clear(void);
	static void GenerateTangent(void);
	int Length;
	char Name[MaxNameLen];
	float maxRadius;
private:
	GLuint vbo[6],vao;
};
	//GLfloat **positions;
	//GLfloat **normals;
	//GLuint *indices;

extern ListContainer<MeshData> Meshes;