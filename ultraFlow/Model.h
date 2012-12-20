#pragma once
#include "Base.h"
#include "MeshData.h"
#include "TextureData.h"
#include "ShaderData.h"
#include "MaterialData.h"
//#include "Scene.h"
//#include "ListContainer.h"

class Model : public GameBase
{
private:
	virtual void setupMatrices(void);
protected:
	mat4 modelMatrix;
	vec3 oldPosition;
	vec3 oldRotation;
	//Scene* scene;
public:
	MeshData *Mesh;
	MaterialData *Material;
	Model();
	//Model(Scene* scene);
	virtual ~Model(void);
	ListContainer<UniformInsert> UniformInserts;
	virtual void Draw(enum DrawingPass pass);
	virtual void Draw();
	virtual void Update(void);
	//void AppendTextureData(TextureData* newTex);
};



