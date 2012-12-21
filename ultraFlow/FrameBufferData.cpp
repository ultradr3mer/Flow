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
	depthBufferFmt = GL_DEPTH_COMPONENT16;
}

FrameBufferData::~FrameBufferData(void)
{
	glDeleteTextures(1, &TextureId);
	glDeleteTextures(1, &DepthTextureId);
	glDeleteFramebuffersEXT(1, &FboId);
}

void FrameBufferData::Bind()
{
	if(FboReady)
	{
		// set rendering destination to FBO
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, FboId);
		glViewport(0, 0, SizeX, SizeY);

		// clear buffers
		glClearColor(Clearcolor.r,Clearcolor.g,Clearcolor.b,Clearcolor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}

void FrameBufferData::Initialize()
{
	updateError("FrameBufferData ENTER Initialize");

	if(UseColor)
	{
		// Create Color Tex
		if(TextureId == -1)
			glGenTextures(1, &TextureId);
		glBindTexture(GL_TEXTURE_2D, TextureId);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, SizeX, SizeY, 0,
					GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// Create Depth Tex
	if(DepthTextureId == -1)
		glGenTextures(1, &DepthTextureId);
	glBindTexture(GL_TEXTURE_2D, DepthTextureId);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, depthBufferFmt, SizeX, SizeY, 0,
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

void DefaultFrameBuffer::Bind()
{
	// unbind FBO
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glViewport(0, 0, screenX, screenY);
	//glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
}
#pragma endregion

#pragma region BufferSet
BufferSet::BufferSet(int x, int y)
{
	NormalPass.SizeX = x;
	NormalPass.SizeY = y;
	NormalPass.depthBufferFmt = GL_DEPTH_COMPONENT24;
	NormalPass.Initialize();

	DefferedLightmap.SizeX = x;
	DefferedLightmap.SizeY = y;
	DefferedLightmap.Initialize();

	ScenePass.SizeX = x;
	ScenePass.SizeY = y;
	ScenePass.Initialize();

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
	"Scene"
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
		default:
			break;
		}

		if(textureId != -1)
		{
			glActiveTexture(GL_TEXTURE0 + CurTexUnit);
			glBindTexture(GL_TEXTURE_2D, textureId);
			ShaderData::Uniform1i(Target, CurTexUnit);
			CurTexUnit++;
		}
	}
}
#pragma endregion