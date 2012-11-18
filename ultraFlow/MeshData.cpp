#include "MeshData.h"

const int buffersize =128000;
GLfloat positionBuffer[buffersize][3];
GLfloat textureBuffer[buffersize][2];
GLfloat normalBuffer[buffersize][3];
GLuint indexBuffer[buffersize];

float indexedPositionBuffer[buffersize*3];
float indexedtextureBuffer[buffersize*2];
float indexedNormalBuffer[buffersize*3];

int dataLenght = 0;
int dataLenghtIndex = 0;

MeshData::MeshData(void)
{

	//GLchar *vertexsource, *fragmentsource;

    /* Allocate and assign a Vertex Array Object to our handle */
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
 
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
}

MeshData* MeshData::FromObj(char* source)
{
	char* data = FileToBuf(FullFileName(source));

	//int readerPos;
	//int length;
	char line[256];
	char subline[128];
	char subline2[64];
	float tmpPos[3];
	float tmpTex[2];
	float tmpNor[3];
	int tmpIndex;
	int curPos = 0;
	int curTex = 0;
	int curNor = 0;
	bool assigned = false;
	int lineCounter = 0;
	int wholeIndex = 0, lineIndex = 0, sublineIndex = 0;

	while(cutFromChar(line,'\n', data, &wholeIndex))
	{
		lineCounter ++;
		lineIndex = 0;
		cutFromChar(subline,' ', line, &lineIndex);

		// line contains vertex position
		if (!strcmp(subline,"v"))
		{
			cutFromChar(subline,' ', line, &lineIndex);
			indexedPositionBuffer[curPos] = (float)strtod(subline,NULL);
			cutFromChar(subline,' ', line, &lineIndex);
			indexedPositionBuffer[curPos+1] = (float)strtod(subline,NULL);
			cutFromChar(subline,' ', line, &lineIndex);
			indexedPositionBuffer[curPos+2] = (float)strtod(subline,NULL);

			curPos += 3;
		}
		// line contains vertex texture
		else if(!strcmp(subline,"vt"))
		{
			cutFromChar(subline,' ', line, &lineIndex);
			indexedtextureBuffer[curTex] = (float)strtod(subline,NULL);
			cutFromChar(subline,' ', line, &lineIndex);
			indexedtextureBuffer[curTex+1] = 1-(float)strtod(subline,NULL);

			curTex += 2;
		}
		// line contains vertex normal
		else if(!strcmp(subline,"vn"))
		{
			cutFromChar(subline,' ', line, &lineIndex);
			indexedNormalBuffer[curNor] = (float)strtod(subline,NULL);
			cutFromChar(subline,' ', line, &lineIndex);
			indexedNormalBuffer[curNor+1] = (float)strtod(subline,NULL);
			cutFromChar(subline,' ', line, &lineIndex);
			indexedNormalBuffer[curNor+2] = (float)strtod(subline,NULL);

			curNor += 3;
		}
		// line contains face
		else if(!strcmp(subline,"f"))
		{
			while (cutFromChar(subline,' ', line, &lineIndex))
			{
				sublineIndex = 0;
				assigned = false;

				//parse position/texture/normal
				cutFromChar(subline2,'/', subline, &sublineIndex);
				tmpIndex = (int)strtod(subline2,NULL)-1;
				tmpPos[0] = indexedPositionBuffer[tmpIndex*3];
				tmpPos[1] = indexedPositionBuffer[tmpIndex*3+1];
				tmpPos[2] = indexedPositionBuffer[tmpIndex*3+2];
				cutFromChar(subline2,'/', subline, &sublineIndex);
				tmpIndex = (int)strtod(subline2,NULL)-1;
				tmpTex[0] = indexedtextureBuffer[tmpIndex*2];
				tmpTex[1] = indexedtextureBuffer[tmpIndex*2+1];
				cutFromChar(subline2,'/', subline, &sublineIndex);
				tmpIndex = (int)strtod(subline2,NULL)-1;
				tmpNor[0] = indexedNormalBuffer[tmpIndex*3];
				tmpNor[1] = indexedNormalBuffer[tmpIndex*3+1];
				tmpNor[2] = indexedNormalBuffer[tmpIndex*3+2];

				// check if simmilar vertex exists and assign corresponding index
				for (int i = 0; i < dataLenght; i++)
				{
					if(//check if pos is the same
						tmpPos[0] == positionBuffer[i][0] &&
						tmpPos[1] == positionBuffer[i][1] &&
						tmpPos[2] == positionBuffer[i][2] &&
						//check if texture is the same
						tmpTex[0] == textureBuffer[i][0] &&
						tmpTex[1] == textureBuffer[i][1] &&
						//check if normal is the same
						tmpNor[0] == normalBuffer[i][0] &&
						tmpNor[1] == normalBuffer[i][1] &&
						tmpNor[2] == normalBuffer[i][2])
					{
						indexBuffer[dataLenghtIndex++] = i;
						assigned = true;
						break;
					}
				}

				// if there is no simmilar vertex create new one
				if(!assigned)
				{
					//save position data
					positionBuffer[dataLenght][0] = tmpPos[0];
					positionBuffer[dataLenght][1] = tmpPos[1];
					positionBuffer[dataLenght][2] = tmpPos[2];
					//save texture data
					textureBuffer[dataLenght][0] = tmpTex[0];
					textureBuffer[dataLenght][1] = tmpTex[1];
					//save normal
					normalBuffer[dataLenght][0] = tmpNor[0];
					normalBuffer[dataLenght][1] = tmpNor[1];
					normalBuffer[dataLenght][2] = tmpNor[2];

					//save index
					indexBuffer[dataLenghtIndex++] = dataLenght++;
				}
			}
		}
	}

	free(data);
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
	glBindVertexArray(vao);
}