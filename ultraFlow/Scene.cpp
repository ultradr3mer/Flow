#include "Scene.h"

Scene* curScene;

Scene::Scene(void)
{
	//Lists used for rendering not storing
	Lights.PerformCleanup = false;
	SceneDrawables.PerformCleanup = false;

	sun = new Sun();
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

void drawShadowPass()
{
	curScene->Draw(DrawingPassShadow);
}

void Scene::DrawShadowBuffers()
{
	glEnable(GL_DEPTH_TEST); 
	Sun* light = nullptr;
	Lights.InitReader(&light);
	while (Lights.Read())
	{

		light->UpdateShadowBuffer(drawShadowPass);
		//Draw(DrawingPassShadow);
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

void Scene::Update()
{
	sun->Position = EyePos;
}