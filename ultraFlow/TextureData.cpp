#include "TextureData.h"
#include "DDSLoader.h"
#include "ShaderData.h"

int CurTexUnit;

TextureData* TextureCache[4096];
char* TextureNames[4096];
int TextureCachePosition;

TextureData::TextureData(void)
{
	// allocate a texture name
	glGenTextures( 1, &textureId );
	// select our current texture
	glBindTexture( GL_TEXTURE_2D, textureId );
	// set the minification filter
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
	// set the magnification filter
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
}

TextureData::~TextureData(void)
{
}

void TextureData::Bind(void)
{
	glActiveTexture(GL_TEXTURE0 + CurTexUnit);
	glBindTexture(GL_TEXTURE_2D, textureId);
	ShaderData::Uniform1i(Target, CurTexUnit);
	CurTexUnit++;
}

TextureData* TextureData::FromDDS(char* source)
{
	for (int i = 0; i < TextureCachePosition; i++)
	{
		if(strcmp(TextureNames[i],source) == 0)
			return TextureCache[i];
	}

	char* name = new char[strlen(source)];
	strcpy(name,source);
	TextureNames[TextureCachePosition] = name;

	TextureCache[TextureCachePosition] = new TextureData();
	DDSLoader::loadDds(source);

	return TextureCache[TextureCachePosition++];
}

TextureData* TextureData::SetTarget(enum Uniforms target)
{
	Target = target;
	return this;
}