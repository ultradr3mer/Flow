#include "Scene.h"

Scene* curScene;

Scene::Scene(void)
{
	//Lists used for rendering not storing
	Lights.PerformCleanup = false;
	SceneDrawables.PerformCleanup = false;

	sun = new Sun();
	Lights.Add(sun);

	SsaoUniforms.PerformCleanup = true;

	// Setup filter
	filter2D = new Filter2D();

	//Setup filters
	BloomCurveFilter = MaterialData::FromXml("bloomCurve.xmf");
	BloomBlurFilter = MaterialData::FromXml("bloomBlur.xmf");
	BloomBlurFilterB = MaterialData::FromXml("bloomBlurB.xmf");
	CompositeFilter = MaterialData::FromXml("composite.xmf");

	SsaoPrepare = MaterialData::FromXml("ssaoPrepare.xmf");
	SsaoPerform = MaterialData::FromXml("ssaoPerform.xmf");
	SsaoBlur = MaterialData::FromXml("ssaoBlur.xmf");
	SsaoMult = MaterialData::FromXml("ssaoMult.xmf");

	//Set Ssao Uniforms
	SsaoUniforms.Add(new UniformInsert(MatViewProjection,DataTypeMat4,&InvertedViewProjectionMat));
	SsaoUniforms.Add(new UniformInsert(FltNear,DataType1f,&NearZ));
	SsaoUniforms.Add(new UniformInsert(FltFar,DataType1f,&FarZ));
}


Scene::~Scene(void)
{
}

void Scene::DrawPass(enum DrawingPass pass)
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
	curScene->DrawPass(DrawingPassShadow);
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

void Scene::DrawScene(BufferSet* bufferSet)
{
	//Set Values
	NearZ = curViewPort->Near;
	FarZ = curViewPort->Far;
	CurrentBufferSet = bufferSet;
	InvertedViewProjectionMat = inverse(curViewPort->InvViewMat);

	//Draw deffered pass
	bufferSet->NormalPass.Bind(true);
	glEnable(GL_DEPTH_TEST); 
	DrawPass(DrawingPassDeffered);

	//Calculate SSAO
	glDisable(GL_DEPTH_TEST); 
	bufferSet->SsaoPrepare.Bind(false);
	filter2D->Draw(SsaoPrepare,&SsaoUniforms);
	bufferSet->SsaoPerform.Bind(false);
	filter2D->Draw(SsaoPerform,&SsaoUniforms);
	bufferSet->SsaoBlur.Bind(false);
	filter2D->Draw(SsaoBlur,&SsaoUniforms);

	//Draw Lights
	//bufferSet->OutBuffer->Bind();
	bufferSet->DefferedLightmap.Bind(true);
	DrawDefferedLights();

	//Draw solid pass
	glEnable(GL_DEPTH_TEST);
	bufferSet->ScenePass.Bind(true);
	DrawPass(DrawingPassSolid);

	//Draw transparent pass
	glDisable(GL_DEPTH_TEST); 
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	filter2D->Draw(SsaoMult);
	DrawPass(DrawingPassTransparent);
	glDisable(GL_BLEND);

	//Calulate Bloom
	bufferSet->Bloom.SetMultiSampeling(false);
	bufferSet->Bloom.Bind(false);
	filter2D->Draw(BloomCurveFilter);
	bufferSet->BloomB.Bind(false);
	filter2D->Draw(BloomBlurFilter);
	bufferSet->Bloom.Bind(false);
	filter2D->Draw(BloomBlurFilterB);
	//bufferSet->BloomB.Bind();
	//filter2D->Draw(scene->BloomBlurFilter);
	//bufferSet->Bloom.Bind();
	//filter2D->Draw(scene->BloomBlurFilterB);

	//Output final image
	bufferSet->OutBuffer->Bind(false);
	bufferSet->Bloom.SetMultiSampeling(true);
	filter2D->Draw(CompositeFilter);
	//filter2D->Draw(SsaoBlur,&SsaoUniforms);
	//filter2D->Draw(SsaoPerform,&SsaoUniforms);
}