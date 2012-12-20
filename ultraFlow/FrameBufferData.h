#pragma once
#include "Base.h"
#include "ShaderData.h"
#include "GenFunc.h"
#include "TextureData.h"

#pragma region FrameBufferData
class FrameBufferData
{
public:
	//settings
	int SizeX;
	int SizeY;
	vec4 Clearcolor;

	//info
	GLuint TextureId;
	GLuint DepthTextureId;
	GLuint FboId;
	bool FboReady;
	GLint depthBufferFmt;

	//creation
	FrameBufferData(void);
	~FrameBufferData(void);
	void Initialize();
	virtual void Bind();
};

//GL window framebuffer
class DefaultFrameBuffer : FrameBufferData
{
public:
	DefaultFrameBuffer();
	virtual void Bind();
};

extern DefaultFrameBuffer defaultFramebuffer;
#pragma endregion

#pragma region BufferSet
class BufferSet
{
public:
	// Used access result
	FrameBufferData* OutBuffer;
	FrameBufferData NormalPass;
	FrameBufferData DefferedLightmap;

	//Functions
	BufferSet(int x, int y);
	~BufferSet();
};

extern BufferSet* CurrentBufferSet;
#pragma endregion

#pragma region Textures
enum FrameBuffer
{
	FrameBufferNull,
	FrameBufferNormalColor,
	FrameBufferNormalDepth,
	FrameBufferDefferedLightmap,
	MyShadowmap
};

struct FbTextureBinder : TextureData
{
	enum FrameBuffer Texture;
	enum Uniforms Target;

	FbTextureBinder(char* target, char* texture);
	virtual void Bind();
};
#pragma endregion