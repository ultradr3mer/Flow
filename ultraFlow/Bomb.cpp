#include "Bomb.h"


Bomb::Bomb(vec3* startPos)
{
	state = 0.0f;

	model = new Model();
	model->Mesh = MeshData::FromObj("plane.obj");
	model->Shader = ShaderData::FromPlainText("Bomb.vert","Bomb.frag");
	model->AppendTextureData(TextureData::FromDDS("bomb.dds")->SetTarget(TexDiffuse));

	Position = *startPos;
	model->Position->x = startPos->x;
	model->Position->y = startPos->y;
	model->Position->z = startPos->z;

	model->UniformInserts[0] = UniformInsert(FltState,DataType1f,&state);
	model->UniformInsertCount = 1;

	alive = true;
}


Bomb::~Bomb(void)
{
}

void Bomb::Update()
{
	if(alive)
	{
		state += 0.01f;
		model->Update();
	}
}

void Bomb::Draw()
{
	if(alive)
		model->Draw();
}