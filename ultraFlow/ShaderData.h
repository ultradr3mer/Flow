#pragma once
#include "Base.h"
//#include "ListContainer.h"

//Don't forget to update UniformsStrings
enum Uniforms
{
	UniformsNull,
	TexDiffuse,
	TexDiffuse2,
	TexDiffuse3,
	TexDiffuse4,
	TexDiffuse5,
	TexDiffuse6,
	TexNormal,
	TexShadingInfo,
	TexDepth,
	TexDeffLightmap,
	TexShadowMap,
	TexInnerShadowMap,
	TexDeffReflections,
	MatShadowProjection,
	MatInnerShadowProjection,
	MatModelView,
	MatViewProjection,
	VecOrigin,
	VecSize,
	VecDirection,
	VecRenderSize,
	VecEyePos,
	VecEyeFwd,
	VecEyeRight,
	VecEyeUp,
	VecLight,
	VecAmbient,
	FltNear,
	FltFar,
	FltAspect,
	FltAlpha,
	FltState,
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
	DataType1f,
	DataTypeMat4,
	DataTypeVector3
};

struct UniformInsert
{
	enum Uniforms unifrom;
	enum DataType type;
	void* data;

	inline UniformInsert(){};

	inline UniformInsert(
		enum Uniforms mUnifrom,
		enum DataType mType,
		void* mData)
	{
		unifrom = mUnifrom;
		type = mType;
		data = mData;
	};
};

extern int UniformCount;

class ShaderData
{
public:
	GLuint shaderprogram;
	ShaderData(char* vertexShader, char* fragmentShader);
	~ShaderData(void);
	void Bind(void);
	char VertexName[MaxNameLen];
	char FragmentName[MaxNameLen];
	static ShaderData* FromPlainText(char* vertexSource, char* fragmentSource);
	static void Uniform1i(enum Uniforms, GLint i);
	static void Uniform1f(enum Uniforms, GLfloat f);
	static void Uniform3fv(enum Uniforms, vec3 const & vec);
	static void Uniform2fv(enum Uniforms, vec2 const & vec);
	static void UniformMatrix4fv(enum Uniforms target, mat4 const & matrix);
	static void UniformMatrix4fv(enum Uniforms target, GLfloat* matrix );
	static void ParseUniformInserts(ListContainer<UniformInsert>* list);
	static bool HasUniform(enum Uniforms);
private:
	GLint* UniformLocations;
	void generateLocations();
	GLint getLocation(enum Uniforms target);
	GLuint vertexshader, fragmentshader;
	GLchar *vertexsource, *fragmentsource;
};

enum Uniforms GetUniformEnum(char*);

extern ListContainer<ShaderData> Shaders;