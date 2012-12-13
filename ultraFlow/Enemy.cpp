#include "Enemy.h"

Enemy* enemys[256];
int curEnemyCount;
float enemySpawnTime;
float enemysPerSecond;

float speed = 0.01f;
float initialWaveLength = 5.0f;
float waveAmplitude  = 0.2f;

Enemy::Enemy(void)
{
	model = new Model();
	model->Mesh = MeshData::FromObj("plane.obj");
	model->Shader = ShaderData::FromPlainText("textureSimple.vert","textureSimple.frag");
	model->AppendTextureData(TextureData::FromDDS("enemy.dds")->SetTarget(TexDiffuse));

	speed += 0.0004f;
	initialWaveLength *= 0.98f;
	waveAmplitude  += 0.04f;
	waveLength = initialWaveLength;

	alive = true;

	basePos = linearRand(-10.0f,10.0f);

	model->Position->z = -10;
	model->Rotation->y = -0.25;

	Update();
}


Enemy::~Enemy(void)
{
	delete model;
}

void Enemy::Update()
{
	if(alive)
	{
		model->Position->z += speed;
		model->Position->x = basePos + sin((model->Position->z - 463.54f) / waveLength) * waveAmplitude;
		model->Update();
		Position = *model->Position;

		if(model->Position->z > 10) alive = false;
	}
}

void Enemy::Draw()
{
	if(alive)
		model->Draw();
}