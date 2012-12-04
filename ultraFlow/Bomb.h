#pragma once
#include "Model.h"
class Bomb
{
private:
	Model* model;
	vec3 Position;
	GLfloat state;
public:
	bool alive;
	Bomb(vec3* startPos);
	~Bomb(void);
	void Draw();
	void Update();
};

