#pragma once
#include "model.h"
#include "ViewPortIso.h"
//#include "Scene.h"
//#include "ListContainer.h"

typedef void (*drawShadowCall)(void);

class Sun :
	public Model
{
private:
	mat4 shadowProjectionMatrix;
	mat4 shadowInnerProjectionMatrix;
public:
	FrameBufferData ShadowBuffer;
	FrameBufferData ShadowInnerBuffer;
	ViewPort* viewPort;
	ViewPort* viewInnerPort;
	Sun(void);
	~Sun(void);
	virtual void Draw();
	virtual void Update();
	void UpdateShadowBuffer(drawShadowCall call);
};

extern Sun* CurLight;