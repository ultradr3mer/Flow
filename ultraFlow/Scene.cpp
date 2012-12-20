#include "Scene.h"

Scene::Scene(void)
{
	//Lists used for rendering not storing
	Lights.PerformCleanup = false;
	SceneDrawables.PerformCleanup = false;

	Sun* sun = new Sun();
	Lights.Add(sun);
}


Scene::~Scene(void)
{
}

void Scene::Draw(enum DrawingPass pass)
{
	Model* model = nullptr;
	SceneDrawables.InitReader(&model);
	while (SceneDrawables.Read())
	{
		model->Draw(pass);
	}
}

void Scene::DrawShadowBuffers()
{
	glEnable(GL_DEPTH_TEST); 
	Sun* light = nullptr;
	Lights.InitReader(&light);
	while (Lights.Read())
	{
		light->Bind();
		Draw(DrawingPassShadow);
	}
}

void Scene::DrawDefferedLights()
{
	Sun* light;
	Lights.InitReader(&light);
	while (Lights.Read())
	{
		CurLight = light;
		light->Draw();
	}
}