#include "Base.h"

#pragma once

//Don't forget to update UniformsStrings
enum Uniforms
{
	TexDiffuse,
	TexNormal,
	MatModelView,
	MatViewProjection
};

class ShaderData
{
public:
	GLuint shaderprogram;
	ShaderData(char* vertexShader, char* fragmentShader);
	~ShaderData(void);
	void Bind(void);
	static ShaderData* FromPlainText(char* vertexSource, char* fragmentSource);
	static void Uniform1i(enum Uniforms, GLint i);
	static void UniformMatrix4fv(enum Uniforms target, mat4 const & matrix);
private:
	int uniformLocationsLength;
	pair <enum Uniforms,GLint>* UniformLocations;
	void generateLocations();
	GLint getLocation(enum Uniforms target);
	GLuint vertexshader, fragmentshader;
	GLchar *vertexsource, *fragmentsource;
};

