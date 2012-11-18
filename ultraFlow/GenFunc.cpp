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