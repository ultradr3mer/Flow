#include "FrameBufferData.h"
#include "Sun.h"
BufferSet* CurrentBufferSet;

#pragma region FrameBufferData
//FrameBufferData
FrameBufferData::FrameBufferData(void)
{
	FboId = -1;
	DepthTextureId = -1;
	TextureId = -1;
	Clearcolor = vec4(0,0,0,0);
	DepthBufferFmt = GL_DEPTH_COMPONENT16;
	BufferFmt = GL_RGBA8;
	MultiSampeling = false;
}

FrameBufferData::~FrameBufferData(void)
{
	glDeleteTextures(1, &TextureId);
	glDeleteTextures(1, &DepthTextureId);
	glDeleteFramebuffersEXT(1, &FboId);
}

void FrameBufferData::Bind(bool clear)
{
	if(FboReady)
	{
		// set rendering destination to FBO
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, FboId);
		glViewport(0, 0, SizeX, SizeY);

		// clear buffers
		if(clear)
		{
			glClearColor(Clearcolor.r,Clearcolor.g,Clearcolor.b,Clearcolor.a);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
	}
}

void FrameBufferData::SetMultiSampeling(bool value)
{
	MultiSampeling = value;
	GLfloat param = MultiSampeling ? GL_LINEAR : GL_NEAREST;

	if(UseColor)
	{
		glBindTexture(GL_TEXTURE_2D, TextureId);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param);
	}

	glBindTexture(GL_TEXTURE_2D, DepthTextureId);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param);
};

void FrameBufferData::Initialize()
{
	updateError("FrameBufferData ENTER Initialize");

	GLfloat sampeling = MultiSampeling ? GL_LINEAR : GL_NEAREST;

	if(UseColor)
	{
		// Create Color Tex
		if(TextureId == -1)
			glGenTextures(1, &TextureId);
		glBindTexture(GL_TEXTURE_2D, TextureId);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, sampeling);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, sampeling);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, BufferFmt, SizeX, SizeY, 0,
					GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// Create Depth Tex
	if(DepthTextureId == -1)
		glGenTextures(1, &DepthTextureId);
	glBindTexture(GL_TEXTURE_2D, DepthTextureId);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, sampeling);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, sampeling);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, DepthBufferFmt, SizeX, SizeY, 0,
		GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);


	// create a framebuffer object
	if(FboId == -1)
		glGenFramebuffersEXT(1, &FboId);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, FboId);

	GLenum status = GL_FRAMEBUFFER_COMPLETE_EXT;
	if(UseColor)
	{
		// attach the texture to FBO color attachment point
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D, TextureId, 0);
		GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	}
	else
	{
		// Instruct openGL that we won't bind a color texture with the currently bound FBO
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}

	// attach the depth exture to depth attachment point
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,GL_TEXTURE_2D, DepthTextureId, 0);
	GLenum FBOstatus = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);

	// check FBO status
	if(status == GL_FRAMEBUFFER_COMPLETE_EXT && FBOstatus == GL_FRAMEBUFFER_COMPLETE_EXT)
		FboReady = true;
	else
		FboReady = false;

	// switch back to window-system-provided framebuffer
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	// check error
	updateError("FrameBufferData Initialize");
}

//DefaultFrameBuffer
DefaultFrameBuffer defaultFramebuffer; 

DefaultFrameBuffer::DefaultFrameBuffer()
{
}

void DefaultFrameBuffer::Bind(bool clear)
{
	// unbind FBO
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glViewport(0, 0, screenX, screenY);

	if(clear)
	{
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	//glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
}
#pragma endregion

#pragma region BufferSet
BufferSet::BufferSet(int x, int y)
{
	NormalPass.SizeX = x;
	NormalPass.SizeY = y;
	NormalPass.DepthBufferFmt = GL_DEPTH_COMPONENT24;
	NormalPass.Initialize();

	DefferedLightmap.SizeX = x;
	DefferedLightmap.SizeY = y;
	DefferedLightmap.Initialize();

	ScenePass.SizeX = x;
	ScenePass.SizeY = y;
	ScenePass.Initialize();

	Bloom.SizeX = x/4;
	Bloom.SizeY = y/4;
	Bloom.Initialize();

	BloomB.SizeX = x/4;
	BloomB.SizeY = y/4;
	BloomB.Initialize();

	SsaoPrepare.SizeX = x;
	SsaoPrepare.SizeY = y;
	SsaoPrepare.BufferFmt = GL_RGBA16;
	SsaoPrepare.Initialize();

	SsaoPerform.SizeX = x/4;
	SsaoPerform.SizeY = y/4;
	SsaoPerform.Initialize();

	SsaoBlur.SizeX = x/4;
	SsaoBlur.SizeY = y/4;
	SsaoBlur.MultiSampeling = true;
	SsaoBlur.Initialize();

	OutBuffer = (FrameBufferData*)&defaultFramebuffer;
}

BufferSet::~BufferSet()
{
}
#pragma endregion

#pragma region Textures
char* BufferNames[] = {
	"NULL",
	"NormalColor",
	"NormalDepth",
	"DefferedLightmap",
	"MyShadowmap",
	"MyInnerShadowMap",
	"Scene",
	"Bloom",
	"BloomB",
	"SsaoPrepare",
	"SsaoPerform",
	"SsaoBlur"
};

int BufferNameCount = sizeof(BufferNames)/sizeof(BufferNames[0]);

FbTextureBinder::FbTextureBinder(char* target, char* texture)
{
	// Assign Uniform Tartget
	Target = GetUniformEnum(target);

	// Assign Texture Target
	Texture = FrameBufferNull;
	for (int i = 0; i < BufferNameCount; i++)
	{
		if(strcasecmp(BufferNames[i],texture))
		{
			Texture = (FrameBuffer)i;
			break;
		}
	}
}

void FbTextureBinder::Bind()
{
	if(ShaderData::HasUniform(Target))
	{
		GLuint textureId = -1;
		switch (Texture)
		{
		case FrameBufferNormalColor:
			textureId = CurrentBufferSet->NormalPass.TextureId;
			break;
		case FrameBufferNormalDepth:
			textureId = CurrentBufferSet->NormalPass.DepthTextureId;
			break;
		case FrameBufferDefferedLightmap:
			textureId = CurrentBufferSet->DefferedLightmap.TextureId;
			break;
		case MyShadowmap:
			textureId = CurLight->ShadowBuffer.DepthTextureId;
			break;
		case MyInnerShadowMap:
			textureId = CurLight->ShadowInnerBuffer.DepthTextureId;
			break;
		case FrameBufferScene:
			textureId = CurrentBufferSet->ScenePass.TextureId;
			break;
		case Bloom:
			textureId = CurrentBufferSet->Bloom.TextureId;
			break;
		case BloomB:
			textureId = CurrentBufferSet->BloomB.TextureId;
			break;
		case FbSsaoPrepare:
			textureId = CurrentBufferSet->SsaoPrepare.TextureId;
			break;
		case FbSsaoPerform:
			textureId = CurrentBufferSet->SsaoPerform.TextureId;
			break;
		case FbSsaoBlur:
			textureId = CurrentBufferSet->SsaoBlur.TextureId;
			break;
		default:
			break;
		}

		if(textureId != -1 && ShaderData::HasUniform(Target))
		{
			glActiveTexture(GL_TEXTURE0 + CurTexUnit);
			glBindTexture(GL_TEXTURE_2D, textureId);
			ShaderData::Uniform1i(Target, CurTexUnit);
			CurTexUnit++;
		}
	}
}
#pragma endregion