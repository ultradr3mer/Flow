#include "Base.h"
#pragma once
class ViewPort : public GameBase
{
protected:
	bool wasUpdated();
	vec3 posOld;
	vec3 rotOld;
public:
	mat4 ProjectrionMatrix;
	bool CalcReconstrucVecs;
	float Near;
	float Far;
	vec3 fwd;
	vec3 right;
	vec3 up;
	vec3 fwdD;
	vec3 rightD;
	vec3 upD;
	mat4 ViewProjectionMatrix;
	ViewPort(void);
	~ViewPort(void);
	virtual void Update(void);
	void Bind(void);
	vec3 movingVec;
	float Aspect;
};
extern ViewPort* curViewPort;

