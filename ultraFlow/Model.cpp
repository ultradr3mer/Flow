#include "CApp.h"

Model::Model(void)
{
	Textures = nullptr;
}


Model::~Model(void)
{
	delete Mesh;
	delete Shader;
	delete[] Textures;
}

void Model::Draw(void)
{
	Mesh->Bind();
	Shader->Bind();
	int texCount = sizeof(Textures)/sizeof(Textures[0]);
	for (int i = 0; i < texCount; i++)
	{
		Textures[i]->Bind();
	}

	glDrawElements(GL_TRIANGLES, Mesh->Length, GL_UNSIGNED_INT, 0);
}

void Model::AppendTexture(Texture* newTex)
{
	int oldLength = 0;
	if(Textures != nullptr)
		oldLength = sizeof(Textures)/sizeof(Textures[0]);

	int newLength = oldLength+1;
	Texture** newTextures = new Texture*[oldLength+1];
	for (int i = 0; i < oldLength; i++)
	{
		newTextures[i] = Textures[i];
	}
	newTextures[oldLength] = newTex;
	delete[] Textures;
	Textures = newTextures;
}