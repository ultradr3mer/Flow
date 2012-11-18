#include "Base.h"


#pragma once
class TextureData
{
private:
	GLuint textureId;
public:
	TextureData(void);
	~TextureData(void);
	static TextureData* FromDDS(char* source);
	enum Uniforms Target;
	void Bind(void);
	TextureData* SetTarget(enum Uniforms);
};

extern int CurTexUnit;