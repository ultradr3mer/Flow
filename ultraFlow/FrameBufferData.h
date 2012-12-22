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
	GLint DepthBufferFmt;
	GLint BufferFmt;
	bool UseColor;
	bool MultiSampeling;

	//info
	GLuint TextureId;
	GLuint DepthTextureId;
	GLuint FboId;
	bool FboReady;

	//creation
	FrameBufferData(void);
	~FrameBufferData(void);
	void Initialize();
	virtual void Bind(bool clear);
	void SetMultiSampeling(bool value);
};

//GL window framebuffer
class DefaultFrameBuffer : FrameBufferData
{
public:
	DefaultFrameBuffer();
	virtual void Bind(bool clear);
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
	FrameBufferData ScenePass;
	FrameBufferData DefferedLightmap;

	FrameBufferData Bloom;
	FrameBufferData BloomB;

	FrameBufferData SsaoPrepare;
	FrameBufferData SsaoPerform;
	FrameBufferData SsaoBlur;

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
	MyShadowmap,
	MyInnerShadowMap,
	FrameBufferScene,
	Bloom,
	BloomB,
	FbSsaoPrepare,
	FbSsaoPerform,
	FbSsaoBlur
};

struct FbTextureBinder : TextureData
{
	enum FrameBuffer Texture;
	enum Uniforms Target;

	FbTextureBinder(char* target, char* texture);
	virtual void Bind();
};
#pragma endregion