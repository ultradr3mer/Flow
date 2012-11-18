#include "Base.h"
#include "MatrixGenerator.h"
#pragma once
class ViewPort : public GameBase
{
private:
	mat4 ProjectrionMatrix;
	vec3 smoothMove;
public:
	vec3 fwd;
	vec3 right;
	MatrixGenerator* innerMatrixGenerator;
	mat4 ViewProjectionMatrix;
	ViewPort(void);
	~ViewPort(void);
	virtual void Update(void);
	void Bind(void);
	vec3 movingVec;
};
extern ViewPort* curViewPort;

