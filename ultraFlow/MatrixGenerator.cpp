#include "MatrixGenerator.h"


MatrixGenerator::MatrixGenerator(void)
{
	Position = new vec3(0,0,0);
	Rotation = new vec3(0,0,0);
	//Matrix = new mat4();
}

MatrixGenerator::MatrixGenerator(vec3* initPosition,vec3* initRotation)
{
	Position = initPosition;
	Rotation = initRotation;
	//Matrix = new mat4();
	Update();
}

void MatrixGenerator::Update()
{
	Rotation->x -= (int)Rotation->x;
	Rotation->y -= (int)Rotation->y;
	Rotation->z -= (int)Rotation->z;

	Matrix = translate(mat4(1.0f),*Position);
	Matrix = rotate( Matrix, Rotation->y * 360, vec3(0.0f, 1.0f, 0.0f));
	Matrix = rotate( Matrix, Rotation->x * 360, vec3(1.0f, 0.0f, 0.0f));
	Matrix = rotate( Matrix, Rotation->z * 360, vec3(0.0f, 0.0f, 1.0f));
}

MatrixGenerator::~MatrixGenerator(void)
{
}
