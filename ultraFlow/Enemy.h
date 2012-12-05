#pragma once
#include "Model.h"

class Enemy
{
private:
	Model* model;
	//float speed;
	float waveLength;
	//float waveAmplitude;
	float basePos;
public:
	vec3 Position;
	bool alive;
	Enemy(void);
	~Enemy(void);
	void Draw();
	void Update();
};

extern Enemy* enemys[256];
extern int curEnemyCount;
extern float enemySpawnTime;
extern float enemysPerSecond;


