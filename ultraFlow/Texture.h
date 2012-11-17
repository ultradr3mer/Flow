#include "CApp.h"
#pragma once
class Texture
{
private:
	GLuint textureId;
public:
	Texture(void);
	~Texture(void);
	static void resetTextureUnit();
	static Texture* FromDDS(char* source);
	enum Uniforms Target;
	void Bind(void);
	Texture* SetTarget(enum Uniforms);
};


