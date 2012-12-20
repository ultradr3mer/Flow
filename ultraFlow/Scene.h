#pragma once
#include "Model.h"
#include "Sun.h"

class Scene
{
public:
	ListContainer<Model> SceneDrawables;
	ListContainer<Sun> Lights;
	Scene(void);
	~Scene(void);
	void Draw(enum DrawingPass pass);
	void DrawShadowBuffers();
	void DrawDefferedLights();
};

