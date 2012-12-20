#include "TextureData.h"
#include "DDSLoader.h"
#include "ShaderData.h"

int CurTexUnit;

//TextureData* TextureCache[4096];
//char* TextureNames[4096];
ListContainer<TextureData> Textures;
int TextureCachePosition;

float Anisotropic = 16;

TextureData::TextureData(void)
{
	//Textures = ListContainer();
	textureId = -1;

	// Add to texture list
	Textures.Add(this);
}

void TextureData::CreateData()
{
	// allocate a texture name
	glGenTextures( 1, &textureId );
	// select our current texture
	glBindTexture( GL_TEXTURE_2D, textureId );
	// set the minification filter
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
	// set the magnification filter
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	// enable anisotropic filtering
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, Anisotropic);
}

TextureData::~TextureData(void)
{
	if(textureId != -1)
		glDeleteTextures( 1, &textureId );

	Textures.Remove(this);
}

void TextureData::Bind(void)
{
	if(ShaderData::HasUniform(Target))
	{
		glActiveTexture(GL_TEXTURE0 + CurTexUnit);
		glBindTexture(GL_TEXTURE_2D, textureId);
		ShaderData::Uniform1i(Target, CurTexUnit);
		CurTexUnit++;
	}
}

TextureData* TextureData::FromDDS(char* source)
{
	TextureData* textureData = nullptr;
	Textures.InitReader(&textureData);
	while(Textures.Read())
	{
		if(strcasecmp(textureData->Name,source))
			return textureData;
	}

	textureData = new TextureData();
	textureData->CreateData();
	DDSLoader::loadDds(source);
	strcpy(textureData->Name,source);
	//Textures.Add(textureData);

	return textureData;
}

TextureData* TextureData::SetTarget(enum Uniforms target)
{
	Target = target;
	return this;
}