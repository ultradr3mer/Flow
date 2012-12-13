#pragma once
#include "Model.h"
#include "ParticleSystem.h"
class Bomb
{
private:
	Model* model;
	vec3 Position;
	GLfloat state;
	bool exploded;
	ParticleSystem* particleSystem;
public:
	bool alive;
	Bomb(vec3* startPos);
	~Bomb(void);
	void Draw();
	void Update();
};

