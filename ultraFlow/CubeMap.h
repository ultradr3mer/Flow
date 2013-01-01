#pragma once
#include "ViewPort.h"
#include "FrameBufferData.h"
#include "gamebase.h"

class CubeMap :
	public GameBase
{
public:
	int Resolution;
	ViewPort viewPort;

	FrameBufferData* curFrameBuffer;
	FrameBufferData FrameBuffers[6];
	int curSide;

	BufferSet* cubemapBufferSet;

	CubeMap(void);
	virtual ~CubeMap(void);
	virtual void Update();
	void Bind();
};

extern CubeMap* CurCubeMap;

