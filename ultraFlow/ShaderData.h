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
	FltAlpha,
	FltState
};

enum AttributeIndices
{
	AttrPosition,
	AttrNormal,
	AttrTexCoord,
	AttrTangent,
	AttrBiNormal,
	AttrAlpha
};

enum DataType
{
	DataType1f
};

struct UniformInsert
{
	enum Uniforms unifrom;
	enum DataType type;
	void* data;

	UniformInsert();

	UniformInsert(
		enum Uniforms mUnifrom,
		enum DataType mType,
		void* mData)
	{
		unifrom = mUnifrom;
		type = mType;
		data = mData;
	};
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
	static void ShaderData::ParseUniformInserts(UniformInsert* list, int length);
private:
	int uniformLocationsLength;
	pair <enum Uniforms,GLint>* UniformLocations;
	void generateLocations();
	GLint getLocation(enum Uniforms target);
	GLuint vertexshader, fragmentshader;
	GLchar *vertexsource, *fragmentsource;
};

