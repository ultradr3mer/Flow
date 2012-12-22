#include "Base.h"
#pragma once
struct PlnEquation
{
	vec3 n;
	float d;

	PlnEquation(){};
	PlnEquation(vec3 point1, vec3 point2, vec3 point3);

	bool Check(vec3 point, float bias);
};

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
	bool FrustumCheck(vec3 position, float radius);
};
extern ViewPort* curViewPort;

extern int DrawnObj;
extern int NotDrawnObj;
