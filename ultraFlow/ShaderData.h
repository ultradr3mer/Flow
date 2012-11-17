#include "CApp.h"
#pragma once

//Don't forget to update UniformsStrings
enum Uniforms
{
	TexDiffuse,
	TexNormal
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
private:
	int uniformLocationsLength;
	pair <enum Uniforms,GLint>* UniformLocations;
	void generateLocations();
	GLint getLocation(enum Uniforms target);
	GLuint vertexshader, fragmentshader;
	GLchar *vertexsource, *fragmentsource;
};

