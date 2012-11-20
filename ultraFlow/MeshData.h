#include "Base.h"
#include "GenFunc.h"

const int buffersize =128000;
extern GLfloat positionBuffer[buffersize][3];
extern GLfloat textureBuffer[buffersize][2];
extern GLfloat normalBuffer[buffersize][3];
extern GLuint indexBuffer[buffersize];
extern float indexedPositionBuffer[buffersize*3];
extern float indexedtextureBuffer[buffersize*2];
extern float indexedNormalBuffer[buffersize*3];
extern btVector3 positions[buffersize];

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
	int Length;
private:
	GLuint vbo[4],vao;
};
	//GLfloat **positions;
	//GLfloat **normals;
	//GLuint *indices;

