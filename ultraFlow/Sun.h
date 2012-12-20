#pragma once
#include "model.h"
#include "ViewPortIso.h"
//#include "Scene.h"
//#include "ListContainer.h"

class Sun :
	public Model
{
private:
	mat4 shadowProjectionMatrix;
public:
	FrameBufferData ShadowBuffer;
	ViewPort* viewPort;
	Sun(void);
	~Sun(void);
	virtual void Draw();
	virtual void Update();
	void Bind();
};

extern Sun* CurLight;