#include "Scene.h"

Scene* curScene;

Scene::Scene(void)
{
	//Lists used for rendering not storing
	Lights.PerformCleanup = false;
	SceneDrawables.PerformCleanup = false;

	SunLight = new Sun();
	Lights.Add(SunLight);

	SsaoUniforms.PerformCleanup = true;

	// Setup filter
	filter2D = new Filter2D();

	//Setup filters
	BloomCurveFilter = MaterialData::FromXml("bloomCurve.xmf");
	BloomBlurFilter = MaterialData::FromXml("bloomBlur.xmf");
	BloomBlurFilterB = MaterialData::FromXml("bloomBlurB.xmf");
	CompositeFilter = MaterialData::FromXml("composite.xmf");
	CubeMapFilter = MaterialData::FromXml("defCubeMap.xmf");

	SsaoPrepare = MaterialData::FromXml("ssaoPrepare.xmf");
	SsaoPerform = MaterialData::FromXml("ssaoPerform.xmf");
	SsaoBlur = MaterialData::FromXml("ssaoBlur.xmf");
	SsaoMult = MaterialData::FromXml("ssaoMult.xmf");
	BloomAdd = MaterialData::FromXml("bloomAdd.xmf");

	//Set Ssao Uniforms
	SsaoUniforms.Add(new UniformInsert(MatModelView,DataTypeMat4,&ViewMat));
	SsaoUniforms.Add(new UniformInsert(FltNear,DataType1f,&NearZ));
	SsaoUniforms.Add(new UniformInsert(FltFar,DataType1f,&FarZ));

	//Set CubeMap Uniforms
	cubeMapUniforms.Add(new UniformInsert(MatViewProjection,DataTypeMat4,&InvertedViewProjectionMat));

	//Setup Cubemap
	cubeMap = new CubeMap;

	//Setup MapData
	Map = new MapData();
	//Map->AddBrush(new Brush(vec3(-10,-10,-1),vec3(20,20,1),1,false));
	//Map->AddBrush(new Brush(vec3(0,0,0),vec3(1,1,1),1,false));
}


Scene::~Scene(void)
{
	delete Map;
}

void Scene::DrawPass(enum DrawingPass pass)
{
	SceneDrawables.InitReader();
	Map->Draw(pass);
	while (SceneDrawables.Read())
	{
		SceneDrawables.Cur->Draw(pass);
	}
}

void drawShadowPass()
{
	curScene->DrawPass(DrawingPassShadow);
}

void Scene::DrawShadowBuffers()
{
	glEnable(GL_DEPTH_TEST); 
	Lights.InitReader();
	while (Lights.Read())
	{

		Lights.Cur->UpdateShadowBuffer(drawShadowPass);
		//Draw(DrawingPassShadow);
	}
}

void Scene::DrawDefferedLights()
{
	Lights.InitReader();
	while (Lights.Read())
	{
		CurLight = Lights.Cur;
		CurLight->Draw();
	}
}

void Scene::Update()
{
	SunLight->Position = EyePos;
	cubeMap->Position = EyePos;
	Sky->Position = EyePos;
}

void Scene::DrawScene(BufferSet* bufferSet)
{
	//Set Values
	NearZ = curViewPort->Near;
	FarZ = curViewPort->Far;
	CurrentBufferSet = bufferSet;
	ViewMat = curViewPort->ViewMat;
	InvertedViewProjectionMat = curViewPort->InvViewProjectionMatrix;

	//Draw deffered pass
	bufferSet->NormalPass.Bind(true);
	glEnable(GL_DEPTH_TEST); 
	glDisable(GL_BLEND);
	DrawPass(DrawingPassDeffered);

	//Draw reflections
	bufferSet->RefletionPass.Bind(true);
	glDisable(GL_BLEND);
	filter2D->Draw(CubeMapFilter,&cubeMapUniforms);

	//Calculate SSAO
	if(bufferSet->EnableSsao)
	{
		bufferSet->SsaoPrepare.Bind(false);
		glDisable(GL_DEPTH_TEST); 
		glDisable(GL_BLEND);
		filter2D->Draw(SsaoPrepare,&SsaoUniforms);
		bufferSet->SsaoPerform.Bind(false);
		filter2D->Draw(SsaoPerform,&SsaoUniforms);
		bufferSet->SsaoBlur.Bind(false);
		filter2D->Draw(SsaoBlur,&SsaoUniforms);
	}

	//Draw Lights
	//bufferSet->OutBuffer->Bind(true);
	bufferSet->DefferedLightmap.Bind(true);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_BLEND);
	DrawDefferedLights();

	//Draw solid pass
	bufferSet->ScenePass.Bind(true);
	glDisable(GL_DEPTH_TEST);
	Sky->Draw(DrawingPassSolidForced);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	DrawPass(DrawingPassSolid);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if(bufferSet->EnableSsao)
		filter2D->Draw(SsaoMult);

	//Draw transparent pass
	glDisable(GL_DEPTH_TEST); 
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	DrawPass(DrawingPassTransparent);

	//Calulate Bloom
	glDisable(GL_BLEND);
	if(bufferSet->EnableBloom)
	{
		bufferSet->Bloom.SetMultiSampeling(false);
		bufferSet->Bloom.Bind(false);
		filter2D->Draw(BloomCurveFilter);
		bufferSet->BloomB.Bind(false);
		filter2D->Draw(BloomBlurFilter);
		bufferSet->Bloom.Bind(false);
		filter2D->Draw(BloomBlurFilterB);
	}

	//Output final image
	bufferSet->OutBuffer->Bind(false);
	bufferSet->Bloom.SetMultiSampeling(true);
	filter2D->Draw(CompositeFilter);

	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_BLEND);
	if(bufferSet->EnableBloom)
		filter2D->Draw(BloomAdd);
}