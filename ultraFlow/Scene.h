#pragma once
#include "Model.h"
#include "Sun.h"
#include "Filter2D.h"
#include "CubeMap.h"

class Scene : public GameBase
{
private:

public:
	MaterialData* BloomCurveFilter;
	MaterialData* CompositeFilter;
	MaterialData* CubeMapFilter;
	ListContainer<UniformInsert> cubeMapUniforms;

	MaterialData* BloomBlurFilter;
	MaterialData* BloomBlurFilterB;
	MaterialData* BloomAdd;

	MaterialData* SsaoPrepare;
	MaterialData* SsaoPerform;
	MaterialData* SsaoBlur;
	MaterialData* SsaoMult;
	ListContainer<UniformInsert> SsaoUniforms;

	CubeMap* cubeMap;

	mat4 ViewMat;
	mat4 InvertedViewProjectionMat;
	float NearZ;
	float FarZ;

	vec3 EyePos;
	ListContainer<Model> SceneDrawables;
	ListContainer<Sun> Lights;
	Scene(void);
	~Scene(void);
	virtual void DrawPass(enum DrawingPass pass);
	void DrawShadowBuffers();
	void DrawDefferedLights();
	virtual void Update(void);
	void DrawScene(BufferSet* bufferSet);

	Filter2D* filter2D;
	Model* Sky;
	Sun* SunLight;
};

extern Scene* curScene;
