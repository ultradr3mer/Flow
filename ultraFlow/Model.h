#include "CApp.h"
#pragma once
class Model
{
public:
	MeshData *Mesh;
	ShaderData *Shader;
	Texture** Textures;
	Model(void);
	~Model(void);
	void Draw(void);
	void AppendTexture(Texture* newTex);
};

