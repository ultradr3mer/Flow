#include "Base.h"

char execPath[512];
int execPathLenght;

//void updateError(void)
//{
//	GLint error;
//	while((error = glGetError()) != GL_NO_ERROR)
//	{
//		fprintf(stderr, "Error occured");
//	}
//}

char* FullFileName(char* folder, char* file)
{
	execPath[execPathLenght] = '\0';
	strcat(execPath,folder);
	return strcat(execPath,file);
}

void updateError(char* location)
{
	GLint error;
	while((error = glGetError()) != GL_NO_ERROR)
	{
		FILE * pFile = fopen(FullFileName("","log.txt"),"a");
		fprintf(pFile, "Error %d occured at %s\n",error,location);
		printf("Error %d occured at %s\n",error,location);
		fclose(pFile);
	}
}