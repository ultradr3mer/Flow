#include "Base.h"

#pragma once
class MatrixGenerator : public GameBase
{
public:
	mat4 Matrix;
	MatrixGenerator(void);
	MatrixGenerator(vec3* initPosition,vec3* initRotation);
	~MatrixGenerator(void);
	virtual void Update(void);
};

