#include "Base.h"
#include "GenFunc.h"

#pragma once
class MeshData
{
public:
	MeshData(void);
	~MeshData(void);
	void Bind(void);
	static MeshData* FromObj(char* source);
	int Length;
private:
	GLuint vbo[4],vao;
};
	//GLfloat **positions;
	//GLfloat **normals;
	//GLuint *indices;

