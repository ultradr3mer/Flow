#include "Base.h"

#pragma once
class MatrixGenerator
{
private:
	vec3* Position;
	vec3* Rotation;
public:
	mat4* Matrix;
	//MatrixGenerator(void);
	MatrixGenerator(vec3* position,vec3* rotation);
	~MatrixGenerator(void);
	virtual void Update(void);
};

