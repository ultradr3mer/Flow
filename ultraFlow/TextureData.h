#include "Base.h"
//#include "ListContainer.h"

#pragma once
class TextureData
{
private:
	GLuint textureId;
public:
	char Name[64];
	TextureData(void);
	~TextureData(void);
	static TextureData* FromDDS(char* source);
	enum Uniforms Target;
	virtual void Bind(void);
	void CreateData();
	TextureData* SetTarget(enum Uniforms);
};

extern int CurTexUnit;

extern ListContainer<TextureData> Textures;