#include "CubeMap.h"

CubeMap* CurCubeMap;

CubeMap::CubeMap(void)
{
	curSide = 0;
	Resolution = 256;

	cubemapBufferSet = new BufferSet();
	cubemapBufferSet->SizeX = Resolution;
	cubemapBufferSet->SizeY = Resolution;
	cubemapBufferSet->EnableSsao = false;
	cubemapBufferSet->EnableBloom = false;
	cubemapBufferSet->Initialize();

	for (int i = 0; i < 6; i++)
	{
		FrameBuffers[i].SizeX = Resolution;
		FrameBuffers[i].SizeY = Resolution;
		FrameBuffers[i].MultiSampeling = true;
		FrameBuffers[i].Initialize();
	}

	viewPort.Aspect = 1;
	viewPort.FovY = 0.25;
	viewPort.BuildPerspective();

	//GameObjList.Remove(&viewPort);
	//GameObjList.Add(&viewPort);
}


CubeMap::~CubeMap(void)
{
	GameObjList.Remove(this);
	delete cubemapBufferSet;
}

void CubeMap::Update()
{
	curSide ++;
	if(curSide == 6) curSide = 0;

	curFrameBuffer = &FrameBuffers[curSide];
	cubemapBufferSet->OutBuffer = curFrameBuffer;
	viewPort.Position = Position;

	switch (curSide)
	{
	case 0:
		viewPort.Rotation = vec3(0.0f,0.0f,0.0f);
		break;
	case 1:
		viewPort.Rotation = vec3(0.0f,0.25f,0.0f);
		break;
	case 2:
		viewPort.Rotation = vec3(0.0f,0.5f,0.0f);
		break;
	case 3:
		viewPort.Rotation = vec3(0.0f,0.75f,0.0f);
		break;
	case 4:
		viewPort.Rotation = vec3(0.25f,0.0f,0.0f);
		break;
	case 5:
		viewPort.Rotation = vec3(-0.25f,0.0f,0.0f);
		break;
	}
}

void CubeMap::Bind()
{
	CurCubeMap = this;
	viewPort.Bind();
}