#include "Model.h"
#include "ViewPort.h"

Model::Model(void)
{
	Textures = nullptr;
	innerMatrixGenerator = new MatrixGenerator();
	Position = innerMatrixGenerator->Position;
	Rotation = innerMatrixGenerator->Rotation;
	texCount = 0;
}


Model::~Model(void)
{
	delete Mesh;
	delete Shader;
	delete[] Textures;
	delete innerMatrixGenerator;
}

void Model::Draw(void)
{
	Mesh->Bind();
	Shader->Bind();
	for (int i = 0; i < texCount; i++)
	{
		Textures[i]->Bind();
	}

	setupMatrices();

	glDrawElements(GL_TRIANGLES, Mesh->Length, GL_UNSIGNED_INT, 0);
}

void Model::setupMatrices()
{
	ShaderData::UniformMatrix4fv(MatModelView, innerMatrixGenerator->Matrix);
	ShaderData::UniformMatrix4fv(MatViewProjection, curViewPort->ViewProjectionMatrix);
}

void Model::AppendTextureData(TextureData* newTex)
{
	int newLength = texCount+1;
	TextureData** newTextures = new TextureData*[texCount+1];
	for (int i = 0; i < texCount; i++)
	{
		newTextures[i] = Textures[i];
	}
	newTextures[texCount++] = newTex;
	delete[] Textures;
	Textures = newTextures;
}

void Model::Update()
{
	innerMatrixGenerator->Update();
}