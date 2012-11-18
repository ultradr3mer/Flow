#include "Base.h"

void updateError(void)
{
	GLint error;
	while((error = glGetError()) != GL_NO_ERROR)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(error));
	}
}