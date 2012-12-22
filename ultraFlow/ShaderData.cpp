#include "ShaderData.h"
#include "GenFunc.h"
#include "TextureData.h"

static ShaderData* curShader;

//Don't forget to update enum Uniforms
char* UniformsStrings[] = {
	"NULL",
	"Diffuse",
	"Diffuse2",
	"Normal",
	"Depth",
	"DefferedLightmap",
	"Shadowmap",
	"InnerShadowmap",
	"ShadowProjection",
	"InnerShadowProjection",
	"ModelView",
	"ViewProjection",
	"Origin",
	"Direction",
	"RenderSize",
	"EyePos",
	"EyeFwd",
	"EyeRight",
	"EyeUp",
	"NearZ",
	"FarZ",
	"Size",
	"Aspect",
	"Alpha",
	"State"
};

int UniformCount = sizeof(UniformsStrings)/sizeof(UniformsStrings[0]);

ListContainer<ShaderData> Shaders;// = ListContainer();

//ShaderData* ShaderCache[4096];
//char* ShaderNames[4096];
int ShaderCachePosition;

//generates a list with all existing Uniforms and their locations
void ShaderData::generateLocations()
{

	UniformLocations = new GLint[UniformCount];
	for (int i = 0; i < UniformCount; i++)
	{
		UniformLocations[i] = glGetUniformLocation(shaderprogram, UniformsStrings[i]);
	}
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

	glBindAttribLocation(shaderprogram, AttrPosition, "in_Position");
    glBindAttribLocation(shaderprogram, AttrNormal, "in_Normal");
	glBindAttribLocation(shaderprogram, AttrTexCoord, "in_TexCoord");
	glBindAttribLocation(shaderprogram, AttrTangent, "in_Tangent");
	glBindAttribLocation(shaderprogram, AttrBiNormal, "in_BiNormal");
	glBindAttribLocation(shaderprogram, AttrAlpha, "in_Alpha");

    glLinkProgram(shaderprogram);

	generateLocations();

	glUseProgram(0);

	free(vertexsource);
    free(fragmentsource);

	Shaders.Add(this);
}

ShaderData* ShaderData::FromPlainText(char* vertexSource, char* fragmentSource)
{
	ShaderData* Shader;
	Shaders.InitReader(&Shader);
	while(Shaders.Read())
	{
		if(strcmp(Shader->VertexName,vertexSource) == 0 &&
			strcmp(Shader->FragmentName,fragmentSource) == 0)
		{
			return Shader;
		}
	}

	/* Read our shaders into the appropriate buffers */
	char* vertexsource = FileToBuf(FullFileName("shaders\\",vertexSource));
	char* fragmentsource = FileToBuf(FullFileName("shaders\\",fragmentSource));

	Shader = new ShaderData(vertexsource,fragmentsource);
	strcpy(Shader->VertexName,vertexSource);
	strcpy(Shader->FragmentName,fragmentSource);

	return Shader;
}

void ShaderData::Uniform1i(enum Uniforms target, GLint i)
{
	GLint location = curShader->getLocation(target);
	glUniform1i(location, i);
}

void ShaderData::Uniform1f(enum Uniforms target, GLfloat f)
{
	GLint location = curShader->getLocation(target);
	glUniform1f(location, f);
}

void ShaderData::Uniform3fv(enum Uniforms target, vec3 const & vec)
{
	GLint location = curShader->getLocation(target);
	glUniform3fv(location, 1, value_ptr(vec));
}

void ShaderData::Uniform2fv(enum Uniforms target, vec2 const & vec)
{
	GLint location = curShader->getLocation(target);
	glUniform2fv(location, 1, value_ptr(vec));
}

void ShaderData::UniformMatrix4fv(enum Uniforms target, GLfloat* matrix )
{
	GLint location = curShader->getLocation(target);
	glUniformMatrix4fv(location, 1,GL_FALSE, matrix);
}

void ShaderData::UniformMatrix4fv(enum Uniforms target, mat4 const & matrix)
{
	GLint location = curShader->getLocation(target);
	glUniformMatrix4fv(location, 1,GL_FALSE, value_ptr(matrix));
}

GLint ShaderData::getLocation(enum Uniforms target)
{
	return UniformLocations[target];
}

bool ShaderData::HasUniform(enum Uniforms target)
{
	return curShader->UniformLocations[target] != -1;
}

void ShaderData::ParseUniformInserts(ListContainer<UniformInsert>* list)
{
	if(list->Length == 0)
		return;
	
	UniformInsert* insert = nullptr;
	list->InitReader(&insert);
	while(list->Read())
	{
		switch (insert->type)
		{
		case DataType1f:
			Uniform1f(insert->unifrom,*(GLfloat*)insert->data);
			break;
		case DataTypeMat4:
			UniformMatrix4fv(insert->unifrom,value_ptr(*(mat4*)insert->data));
			break;
		default:
			break;
		}
	}

}

ShaderData::~ShaderData(void)
{
	delete[] UniformLocations;
	glDetachShader(shaderprogram, vertexshader);
    glDetachShader(shaderprogram, fragmentshader);
    glDeleteProgram(shaderprogram);
    glDeleteShader(vertexshader);
    glDeleteShader(fragmentshader);

	Shaders.Remove(this);
}

void ShaderData::Bind(void)
{
	curShader = this;
	CurTexUnit = 0;
    glUseProgram(shaderprogram);
}

enum Uniforms GetUniformEnum(char* Name)
{
	for (int i = 0; i < UniformCount; i++)
	{
		if(strcasecmp(UniformsStrings[i],Name))
			return (Uniforms)i;
	}
	return UniformsNull;
}