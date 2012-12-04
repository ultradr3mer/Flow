#include "Enemy.h"


Enemy::Enemy(void)
{
	model = new Model();
	model->Mesh = MeshData::FromObj("gameField.obj");
	model->Shader = ShaderData::FromPlainText("textureSimple.vert","textureSimple.frag");
	model->AppendTextureData(TextureData::FromDDS("gameField.dds")->SetTarget(TexDiffuse));
}


Enemy::~Enemy(void)
{
}
