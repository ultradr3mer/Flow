#include "Base.h"

#pragma once

//Don't forget to update UniformsStrings
enum Uniforms
{
	TexDiffuse,
	TexNormal,
	MatModelView,
	MatViewProjection,
	VecOrigin,
	FltSize,
	FltAspect,
	FltAlpha
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
	static void Uniform1f(enum Uniforms, GLfloat f);
	static void Uniform3fv(enum Uniforms, vec3 const & vec);
	static void UniformMatrix4fv(enum Uniforms target, mat4 const & matrix);
	static void ShaderData::UniformMatrix4fv(enum Uniforms target, GLfloat* matrix );
private:
	int uniformLocationsLength;
	pair <enum Uniforms,GLint>* UniformLocations;
	void generateLocations();
	GLint getLocation(enum Uniforms target);
	GLuint vertexshader, fragmentshader;
	GLchar *vertexsource, *fragmentsource;
};

