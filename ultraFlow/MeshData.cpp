#include "MeshData.h"
#include "ObjLoader.h"

GLfloat positionBuffer[buffersize][3];
GLfloat textureBuffer[buffersize][2];
GLfloat normalBuffer[buffersize][3];
GLuint indexBuffer[buffersize];
float indexedPositionBuffer[buffersize*3];
float indexedtextureBuffer[buffersize*2];
float indexedNormalBuffer[buffersize*3];
btVector3 positions[buffersize];

int dataLenght = 0;
int dataLenghtIndex = 0;

MeshData::MeshData(void)
{

	//GLchar *vertexsource, *fragmentsource;

    /* Allocate and assign a Vertex Array Object to our handle */
	if(useVertexArrays)
	{
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
	}
 
    // generate buffers
    glGenBuffers(4, vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, dataLenght * 3 * sizeof(GLfloat), positionBuffer, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, dataLenght * 3 * sizeof(GLfloat), normalBuffer, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, dataLenght * 2 * sizeof(GLfloat), textureBuffer, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataLenghtIndex * sizeof(GLuint), indexBuffer, GL_STATIC_DRAW);

	Length = dataLenghtIndex;

	Clear();
}

void MeshData::Clear()
{
	dataLenght = 0;
	dataLenghtIndex = 0;
}

MeshData* MeshData::FromObj(char* source)
{
	ObjLoader::Load(source);
	return new MeshData();
}

MeshData::~MeshData(void)
{
	glDeleteBuffers(4, vbo);
	glDeleteVertexArrays(1, &vao);
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

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);
	}
}