#include "GenFunc.h"

/* A simple function that will read a file into an allocated char pointer buffer */
char* FileToBuf(char *file)
{
    FILE *fptr;
    long length;
    char *buf;
 
	fptr = fopen(file, "rb"); /* Open file for reading */
    if (!fptr) /* Return NULL on failure */
        return NULL;
    fseek(fptr, 0, SEEK_END); /* Seek to the end of the file */
    length = ftell(fptr); /* Find out how many bytes into the file we are */
    buf = (char*)malloc(length+1); /* Allocate a buffer for the entire length of the file and a null terminator */
    fseek(fptr, 0, SEEK_SET); /* Go back to the beginning of the file */
    fread(buf, length, 1, fptr); /* Read the contents of the file in to the buffer */
    fclose(fptr); /* Close the file */
    buf[length] = 0; /* Null terminator */
 
    return buf; /* Return the buffer */
}

float max(float valA, float valB)
{
	return valA < valB ? valB : valA;
}
 
bool cutFromChar(char* line, char seperator, char* whole, int* startIndex)
{
	int curIndex = *startIndex;
	if(whole[curIndex] == '\0')
		return false;
	if(curIndex > 0 && whole[curIndex-1] == '\0')
		return false;

	int linePos = 0;
	while (whole[curIndex] != seperator && whole[curIndex] != '\0')
	{
		line[linePos++] = whole[curIndex];
		curIndex++;
	}
	line[linePos] = '\0';
	*startIndex = curIndex + 1;

	return true;
}

mat4 MatrixFromPosAng(vec3 Position, vec3 Rotation)
{
	//Rotation.x -= (int)Rotation.x;
	//Rotation.y -= (int)Rotation.y;
	//Rotation.z -= (int)Rotation.z;

	mat4 outMatrix = translate(mat4(1.0f),Position);
	outMatrix = rotate( outMatrix, Rotation.z * 360, vec3(0.0f, 0.0f, 1.0f));
	outMatrix = rotate( outMatrix, Rotation.y * 360, vec3(0.0f, 1.0f, 0.0f));
	outMatrix = rotate( outMatrix, Rotation.x * 360, vec3(1.0f, 0.0f, 0.0f));
	return outMatrix;
}

mat4 MatrixFromPosAngSiz(vec3 Position, vec3 Rotation, vec3 Size)
{
	//Rotation.x -= (int)Rotation.x;
	//Rotation.y -= (int)Rotation.y;
	//Rotation.z -= (int)Rotation.z;

	mat4 outMatrix = translate(mat4(1.0f),Position);
	outMatrix = rotate( outMatrix, Rotation.z * 360, vec3(0.0f, 0.0f, 1.0f));
	outMatrix = rotate( outMatrix, Rotation.y * 360, vec3(0.0f, 1.0f, 0.0f));
	outMatrix = rotate( outMatrix, Rotation.x * 360, vec3(1.0f, 0.0f, 0.0f));
	outMatrix = scale( outMatrix, Size);
	return outMatrix;
}

bool strcasecmp(char* str1,char* str2)
{
	int length1 = strlen(str1);
	int length2 = strlen(str2);

	if(length1 != length2)
		return false;

	for (int i = 0; i < length1; i++)
	{
		if(toupper(str1[i]) != toupper(str2[i]))
			return false;
	}

	return true;
}

btVector3 BulletVec3FromVec3(vec3* origVector)
{
	return btVector3(origVector->x,origVector->y,origVector->z);
}

vec3 Vec3FromBulletVec3(btVector3* origVector)
{
	return vec3(origVector->x(),origVector->y(),origVector->z());
}

vec3 RotationFromNormal(vec3 normal)
{
	vec3 returnVec(0);
	returnVec.y = atan2(normal.x,normal.z) / pi / 2;
	returnVec.x = atan2(length(vec2(normal.x,normal.z)),normal.y) / pi / 2;
	return returnVec;
}