#include "Enemy.h"


Enemy::Enemy(void)
{
	model = new Model();
	model->Mesh = MeshData::FromObj("plane.obj");
	model->Shader = ShaderData::FromPlainText("textureSimple.vert","textureSimple.frag");
	model->AppendTextureData(TextureData::FromDDS("enemy.dds")->SetTarget(TexDiffuse));

	speed = 0.01f;
	waveLength = 1;
	waveAmplitude  = 1;

	alive = true;

	basePos = linearRand(-10.0f,10.0f);

	model->Position->z = -10;
	model->Rotation->y = -0.25;
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

		if(model->Position->z > 10) alive = false;
	}
}

void Enemy::Draw()
{
	if(alive)
		model->Draw();
}