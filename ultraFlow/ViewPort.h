#include "Base.h"
#pragma once
class ViewPort : public GameBase
{
protected:
	mat4 ProjectrionMatrix;
	bool wasUpdated();
	vec3 posOld;
	vec3 rotOld;
public:
	float Near;
	float Far;
	vec3 fwd;
	vec3 right;
	vec3 up;
	vec3 fwd1;
	vec3 right1;
	vec3 up1;
	mat4 ViewProjectionMatrix;
	ViewPort(void);
	~ViewPort(void);
	virtual void Update(void);
	void Bind(void);
	vec3 movingVec;
	float Aspect;
};
extern ViewPort* curViewPort;

