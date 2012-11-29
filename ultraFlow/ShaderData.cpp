#include "ShaderData.h"
#include "GenFunc.h"
#include "TextureData.h"

static ShaderData* curShader;

//Don't forget to update enum Uniforms
const GLchar* UniformsStrings[] = {
	"Diffuse",
	"Normal",
	"ModelView",
	"ViewProjection"
};

ShaderData* ShaderCache[4096];
char* ShaderNames[4096];
int ShaderCachePosition;

//generates a list with all existing Uniforms and their locations
void ShaderData::generateLocations()
{
	int maxLength = sizeof(UniformsStrings)/sizeof(UniformsStrings[0]);
	int writerPos = 0, curLocation;
	uniformLocationsLength = 0;

	pair <enum Uniforms,GLint> curPair;
	pair <enum Uniforms,GLint>* tmpUniformLocations 
		= new pair <enum Uniforms,GLint>[maxLength];
	for (int i = 0; i < maxLength; i++)
	{
		curLocation = glGetUniformLocation(shaderprogram, UniformsStrings[i]);
		tmpUniformLocations[i] = make_pair ((enum Uniforms)i, curLocation);
		if(curLocation != -1)
		{
			uniformLocationsLength++;
		}
	}

	UniformLocations = new pair <enum Uniforms,GLint>[uniformLocationsLength];
	for (int i = 0; i < maxLength; i++)
	{
		curPair = tmpUniformLocations[i];
		if(curPair.second != -1)
			UniformLocations[writerPos++] = curPair;
	}

	delete[] tmpUniformLocations;
}

ShaderData::ShaderData(char* vertexsource, char* fragmentsource)
{
	char *vertexInfoLog;
    char *fragmentInfoLog;
	int IsCompiled_VS, IsCompiled_FS;
	int maxLength;
	 
    /* Create an empty vertex shader handle */
    vertexshader = glCreateShader(GL_VERTEX_SHADER);
 
    /* Send the vertex shader source code to GL */
    /* Note that the source code is NULL character terminated. */
    /* GL will automatically detect that therefore the length info can be 0 in this case (the last parameter) */
    glShaderSource(vertexshader, 1, (const GLchar**)&vertexsource, 0);
 
    /* Compile the vertex shader */
    glCompileShader(vertexshader);
 
    glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &IsCompiled_VS);
	if(IsCompiled_VS == false)
    {
       glGetShaderiv(vertexshader, GL_INFO_LOG_LENGTH, &maxLength);
 
       /* The maxLength includes the NULL character */
       vertexInfoLog = (char *)malloc(maxLength);
 
       glGetShaderInfoLog(vertexshader, maxLength, &maxLength, vertexInfoLog);
 
       /* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
	   printf(vertexInfoLog);

       /* In this simple program, we'll just leave */
       free(vertexInfoLog);
       return;
    }
 
    /* Create an empty fragment shader handle */
    fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
 
    /* Send the fragment shader source code to GL */
    /* Note that the source code is NULL character terminated. */
    /* GL will automatically detect that therefore the length info can be 0 in this case (the last parameter) */
    glShaderSource(fragmentshader, 1, (const GLchar**)&fragmentsource, 0);
 
    /* Compile the fragment shader */
    glCompileShader(fragmentshader);
 
    glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &IsCompiled_FS);
    if(IsCompiled_FS == false)
    {
       glGetShaderiv(fragmentshader, GL_INFO_LOG_LENGTH, &maxLength);
 
       /* The maxLength includes the NULL character */
       fragmentInfoLog = (char *)malloc(maxLength);
 
       glGetShaderInfoLog(fragmentshader, maxLength, &maxLength, fragmentInfoLog);
 
       /* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
	   printf(fragmentInfoLog);

       /* In this simple program, we'll just leave */
       free(fragmentInfoLog);
       return;
    }

    /* Assign our program handle a "name" */
    shaderprogram = glCreateProgram();
 
    /* Attach our shaders to our program */
    glAttachShader(shaderprogram, vertexshader);
    glAttachShader(shaderprogram, fragmentshader);

	glBindAttribLocation(shaderprogram, 0, "in_Position");
    glBindAttribLocation(shaderprogram, 1, "in_Normal");
	glBindAttribLocation(shaderprogram, 2, "in_TexCoord");
	glBindAttribLocation(shaderprogram, 3, "in_Tangent");
	glBindAttribLocation(shaderprogram, 4, "in_BiNormal");

    glLinkProgram(shaderprogram);

	generateLocations();

	glUseProgram(0);

	free(vertexsource);
    free(fragmentsource);
}

ShaderData* ShaderData::FromPlainText(char* vertexSource, char* fragmentSource)
{
	char* name = new char[strlen(vertexSource)+strlen(fragmentSource)];
	strcpy(name,vertexSource);
	strcat(name,fragmentSource);

	for (int i = 0; i < ShaderCachePosition; i++)
	{
		if(strcmp(ShaderNames[i],name) == 0)
			return ShaderCache[i];
	}

	ShaderNames[ShaderCachePosition] = name;

	/* Read our shaders into the appropriate buffers */
    char* vertexsource = FileToBuf(FullFileName(vertexSource));
	char* fragmentsource = FileToBuf(FullFileName(fragmentSource));

	ShaderCache[ShaderCachePosition] = new ShaderData(vertexsource,fragmentsource);

	return ShaderCache[ShaderCachePosition++];
}

void ShaderData::Uniform1i(enum Uniforms target, GLint i)
{
	GLuint location = curShader->getLocation(target);
	glUniform1i(location, i);
}

void ShaderData::UniformMatrix4fv(enum Uniforms target, GLfloat* matrix )
{
	GLuint location = curShader->getLocation(target);
	glUniformMatrix4fv(location, 1,GL_FALSE, matrix);
}

void ShaderData::UniformMatrix4fv(enum Uniforms target, mat4 const & matrix)
{
	GLuint location = curShader->getLocation(target);
	glUniformMatrix4fv(location, 1,GL_FALSE, value_ptr(matrix));
}

GLint ShaderData::getLocation(enum Uniforms target)
{
	for (int i = 0; i < uniformLocationsLength; i++)
	{
		if(UniformLocations[i].first == target)
			return UniformLocations[i].second;
	}
	return -1;
}

ShaderData::~ShaderData(void)
{
	delete[] UniformLocations;
	glDetachShader(shaderprogram, vertexshader);
    glDetachShader(shaderprogram, fragmentshader);
    glDeleteProgram(shaderprogram);
    glDeleteShader(vertexshader);
    glDeleteShader(fragmentshader);
}

void ShaderData::Bind(void)
{
	curShader = this;
	CurTexUnit = 0;
    glUseProgram(shaderprogram);
}