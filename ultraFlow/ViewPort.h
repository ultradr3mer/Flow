#pragma once
#include "Base.h"
#include "PlnEquation.h"

class ViewPort : public GameBase
{
protected:
	bool wasUpdated();
	vec3 posOld;
	vec3 rotOld;
	PlnEquation clipingPlanes[6];
public:
	mat4 InvViewMat;
	mat4 ViewMat;
	mat4 ProjectrionMatrix;
	mat4 InvViewProjectionMatrix;
	bool CalcReconstrucVecs;
	float Near;
	float Far;
	float FovY;
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
	bool FrustumCheck(vec3 position, float radius);
	void BuildPerspective();
};
extern ViewPort* curViewPort;

extern int DrawnObj;
extern int NotDrawnObj;
