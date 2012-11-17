#include "CApp.h"

int CurTexUnit;

Texture::Texture(void)
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

Texture::~Texture(void)
{
}

void Texture::Bind(void)
{
	glActiveTexture(GL_TEXTURE0 + CurTexUnit);
	glBindTexture(GL_TEXTURE_2D, textureId);
	ShaderData::Uniform1i(Target, CurTexUnit);
	CurTexUnit++;
}

Texture* Texture::FromDDS(char* source)
{
	Texture* tx = new Texture();
	DDSLoader::loadDds(source);
	return tx;
}

void Texture::resetTextureUnit(void)
{
	CurTexUnit = 0;
}

Texture* Texture::SetTarget(enum Uniforms target)
{
	Target = target;
	return this;
}