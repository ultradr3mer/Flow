#pragma once
#include "Model.h"
#include "Sun.h"
#include "Filter2D.h"

class Scene : public GameBase
{
private:
	Sun* sun;
	Filter2D* filter2D;

public:

	MaterialData* BloomCurveFilter;
	MaterialData* BloomBlurFilter;
	MaterialData* BloomBlurFilterB;
	MaterialData* CompositeFilter;

	MaterialData* SsaoPrepare;
	MaterialData* SsaoPerform;
	MaterialData* SsaoBlur;
	MaterialData* SsaoMult;
	ListContainer<UniformInsert> SsaoUniforms;

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
};

extern Scene* curScene;
