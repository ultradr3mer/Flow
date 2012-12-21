#pragma once
#include "Model.h"
#include "Sun.h"

class Scene : public GameBase
{
private:
	Sun* sun;
public:
	vec3 EyePos;
	ListContainer<Model> SceneDrawables;
	ListContainer<Sun> Lights;
	Scene(void);
	~Scene(void);
	virtual void Draw(enum DrawingPass pass);
	void DrawShadowBuffers();
	void DrawDefferedLights();
	virtual void Update(void);
};

extern Scene* curScene;
