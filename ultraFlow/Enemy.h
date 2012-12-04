#pragma once
#include "Model.h"
class Enemy
{
private:
	Model* model;
	float speed;
	float waveLength;
	float waveAmplitude;
	float basePos;
public:
	bool alive;
	Enemy(void);
	~Enemy(void);
	void Draw();
	void Update();
};

