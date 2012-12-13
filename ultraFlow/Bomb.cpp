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

			particleSystem = new ParticleSystem(200);
			particleSystem->AppendTextureData(TextureData::FromDDS("particle.dds")->SetTarget(TexDiffuse));
			particleSystem->BaseAffector->Lifetime = 1;
			particleSystem->BaseAffector->AlphaFunc = AlphaFuncFadeOut;
			particleSystem->ParticleSize = 10;

			ParticleAffectorSpawner* spawner = new ParticleAffectorSpawner();
			spawner->Position = Position;
			spawner->spawnSize = vec3(0.1f);
			spawner->InitialVecRandom = vec3(0.1f,0.1f,0.1f);
			spawner->InitialVec = vec3(0,0.0f,0);
			spawner->particlePerSecond = 0;
			spawner->particlesToSpawn = 200;
			particleSystem->AppendAffector(spawner);

			//ParticleAffectorSpawner* spawner2 = new ParticleAffectorSpawner();
			//spawner2->Position = Position;
			//spawner2->spawnSize = vec3(0.1f);
			//spawner2->InitialVecRandom = vec3(0.05f,0.075f,0.05f);
			//spawner2->InitialVec = vec3(0,0.015f,0);
			//spawner2->particlePerSecond = 0;
			//spawner2->particlesToSpawn = 600;
			//particleSystem->AppendAffector(spawner2);

			//ParticleAffectorGravity* grav = new ParticleAffectorGravity();
			//grav->Strength = -0.0001f;
			//particleSystem->AppendAffector(grav);

			exploded = true;
		}
		if(state > 3.0f)
			alive = false;
		if(exploded)
			particleSystem->Update();
	}
}

void Bomb::Draw()
{
	if(alive)
	{
		if(!exploded)
			model->Draw();
		else
			particleSystem->Draw();
	}
}

