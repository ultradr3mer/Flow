#include "Model.h"
#include "ViewPort.h"

Model::Model(void)
{
	Textures = nullptr;
	innerMatrixGenerator = new MatrixGenerator();
	Position = innerMatrixGenerator->Position;
	Rotation = innerMatrixGenerator->Rotation;
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
	int texCount = sizeof(Textures)/sizeof(Textures[0]);
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
	int oldLength = 0;
	if(Textures != nullptr)
		oldLength = sizeof(Textures)/sizeof(Textures[0]);

	int newLength = oldLength+1;
	TextureData** newTextures = new TextureData*[oldLength+1];
	for (int i = 0; i < oldLength; i++)
	{
		newTextures[i] = Textures[i];
	}
	newTextures[oldLength] = newTex;
	delete[] Textures;
	Textures = newTextures;
}

void Model::Update()
{
	innerMatrixGenerator->Update();
}