#include "Base.h"
#include "MeshData.h"
#include "TextureData.h"
#include "ShaderData.h"
#include "MatrixGenerator.h"

#pragma once
class Model : public GameBase
{
private:
	MatrixGenerator* innerMatrixGenerator;
	virtual void setupMatrices(void);
protected:
	int texCount;
public:
	MeshData *Mesh;
	ShaderData *Shader;
	TextureData** Textures;
	Model(void);
	~Model(void);
	virtual void Draw(void);
	virtual void Update(void);
	void AppendTextureData(TextureData* newTex);
};

