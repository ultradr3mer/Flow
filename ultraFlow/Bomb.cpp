#include "Bomb.h"
#include "Enemy.h"


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
	model->Update();

	model->UniformInserts[0] = UniformInsert(FltState,DataType1f,&state);
	model->UniformInsertCount = 1;

	alive = true;
	exploded = false;
}


Bomb::~Bomb(void)
{
	delete model;
	delete particleSystem;
}

void Bomb::Update()
{
	if(alive)
	{
		state += 0.01f;
		if(state > 1.0f && exploded == false)
		{
			for (int i = 0; i < curEnemyCount; i++)
			{
				float dist  = length(Position - enemys[i]->Position);
				if(enemys[i]->alive && 
					length(Position - enemys[i]->Position) < 1.5f)
					enemys[i]->alive = false;
			}

			particleSystem = new ParticleSystem();
			particleSystem->AppendTextureData(TextureData::FromDDS("particle.dds")->SetTarget(TexDiffuse));
			particleSystem->BaseAffector->Lifetime = 1;

			ParticleAffectorSpawner* spawner = new ParticleAffectorSpawner();
			spawner->Position = &Position;
			spawner->spawnSize = 1.0;
			particleSystem->AppendAffector(spawner);

			ParticleAffectorGravity* grav = new ParticleAffectorGravity();
			grav->Strength = 0.00001f;
			particleSystem->AppendAffector(grav);

			exploded = true;
		}
		if(state > 10.0f)
			alive = false;
		if(exploded)
			particleSystem->Update();
	}
}

void Bomb::Draw()
{
	if(!exploded)
		model->Draw();
	if(exploded)
		particleSystem->Draw();
}

