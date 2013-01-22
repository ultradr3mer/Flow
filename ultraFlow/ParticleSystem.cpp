#include "ParticleSystem.h"
#include "ViewPort.h"

#pragma region ParticleAffectors
void ParticleAffector::Affect(Particle* particles, int count)
{
}

void ParticleAffectorBase::Affect(Particle* particles, int count)
{
	float alpha;
	float step = 1 / Lifetime / 100;
	for (int i = 0; i < count; i++)
	{
		particles[i].Position += particles[i].Vector;
		particles[i].Life -= step;

		switch (AlphaFunc)
		{
		case AlphaFuncFadeOut:
			particles[i].Alpha  = particles[i].Life;
			break;
		case AlphaFuncFadeIn:
			particles[i].Alpha  = 1 - particles[i].Life;
			break;
		case AlphaFuncFadeInOut:
			alpha = particles[i].Life - 0.5f;
			alpha *= sign(alpha);
			alpha = 1 - alpha * 2;
			particles[i].Alpha = alpha;
			break;
		default:
			break;
		}
	}
}

ParticleAffectorSpawner::ParticleAffectorSpawner()
{
	particlesToSpawn = 0;
	particlePerSecond  = 0;
	spawnSize = vec3(2.0f);

	InitialVec = vec3(0);
	InitialVecRandom = vec3(0);
}

void ParticleAffectorSpawner::Affect(Particle* particles, int count)
{
	int curParticlesToSpawn;

	particlesToSpawn += particlePerSecond * 0.01f;
	curParticlesToSpawn = (int)particlesToSpawn;
	particlesToSpawn -= curParticlesToSpawn;


	int SpawnedParticles = 0;
	for (int i = 0; i < count && SpawnedParticles < curParticlesToSpawn; i++)
	{
		if(particles[i].Life <= 0)
		{
			particles[i].Life = 1;
			particles[i].Position = gaussRand(Position, vec3(spawnSize));
			particles[i].Vector = vec3(0);

			particles[i].Vector = InitialVec + gaussRand(vec3(0), vec3(InitialVecRandom));

			SpawnedParticles++;
		}
	}
}

void ParticleAffectorGravity::Affect(Particle* particles, int count)
{
	for (int i = 0; i < count; i++)
	{
		particles[i].Vector.y += Strength;
	}
}
#pragma endregion

#pragma region ParticleSystem
ParticleSystem::ParticleSystem(int MaxParticles)
{
	maxParticles = MaxParticles;
	particles = new Particle[MaxParticles];
	particlePositions = new GLfloat[MaxParticles*3];
	particleAlpha = new GLfloat[MaxParticles];

	//Mesh = MeshData::FromObj("plane.obj");
	//Shader = ShaderData::FromPlainText("Particle.vert","Particle.frag");
	//for (int i = 0; i < particlecount; i++)
	//{
	//	particles[i].Position = gaussRand(vec3(0), vec3(2.5f));
	//	particles[i].Life = linearRand(0.0f,1.0f);
	//}
	affectorCount = 0;

	BaseAffector = new ParticleAffectorBase();
	BaseAffector->Lifetime = 4;
	BaseAffector->AlphaFunc = AlphaFuncFadeInOut;

	AppendAffector(BaseAffector);

	ParticleSize = 10.0f;

	glGenBuffers(2,vbos);
}

ParticleSystem::~ParticleSystem(void)
{
	for (int i = 0; i < affectorCount; i++)
	{
		delete affectors[i];
	}
	delete particles;
	delete particlePositions;
	delete particleAlpha;
	glDeleteBuffers(2,vbos);
}

void ParticleSystem::AppendAffector(ParticleAffector* affector)
{
	affectors[affectorCount++] = affector;
}

void ParticleSystem::Draw(enum DrawingPass pass)
{
	if(pass != DrawingPassTransparent)
		return;
	//Shader->Bind();
	Material->Bind(DrawingPassTransparent);

	//Upload Particle Data
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	glBufferData(GL_ARRAY_BUFFER, curParticleCount * 3 * sizeof(GLfloat), particlePositions, GL_STATIC_DRAW);
	glVertexAttribPointer(AttrPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(AttrPosition);

	glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
	glBufferData(GL_ARRAY_BUFFER, curParticleCount * sizeof(GLfloat), particleAlpha, GL_STATIC_DRAW);
	glVertexAttribPointer(AttrAlpha, 1, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(AttrAlpha);

	glEnable(GL_POINT_SPRITE);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

	//for (int i = 0; i < texCount; i++)
	//{
	//	Textures[i]->Bind();
	//}
	glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, 1);

	ShaderData::UniformMatrix4fv(MatViewProjection, curViewPort->ViewProjectionMatrix);
	ShaderData::UniformMatrix4fv(MatModelView, mat4(1.0f));
	//ShaderData::Uniform1f(FltAspect, curViewPort->Aspect);
	ShaderData::Uniform1f(FltSize, ParticleSize);

	float alpha = 0;
	vec4 position;

	glDrawArrays(GL_POINTS,0,curParticleCount);

	//for (int i = 0; i < particlecount; i++)
	//{
	//	if(particles[i].Life > 0)
	//	{
	//		//position = curViewPort->ViewProjectionMatrix * vec4(particles[i].Position,1);
	//		//position /= position.w;

	//		//if( position.z > 0 &&
	//		//	position.x > -1 &&
	//		//	position.x < 1 &&
	//		//	position.y > -1 &&
	//		//	position.y < 1)
	//		//{
	//			alpha = particles[i].Life - 0.5f;
	//			alpha *= sign(alpha);
	//			alpha = 1 - alpha * 2;
	//			ShaderData::Uniform1f(FltAlpha, alpha);
	//			ShaderData::Uniform3fv(VecOrigin, particles[i].Position);
	//			glDrawElements(GL_TRIANGLES, Mesh->Length, GL_UNSIGNED_INT, 0);
	//		//}
	//	}
	//}
}

void ParticleSystem::Update()
{
	
	for (int i = 0; i < affectorCount; i++)
	{
		affectors[i]->Affect(particles,maxParticles);
	}
	curParticleCount = 0;

	for (int i = 0; i < maxParticles; i++)
	{
		particlePositions[curParticleCount*3+0] = particles[i].Position.x;
		particlePositions[curParticleCount*3+1] = particles[i].Position.y;
		particlePositions[curParticleCount*3+2] = particles[i].Position.z;
		particleAlpha[curParticleCount] = particles[i].Alpha;

		curParticleCount++;
	}
}
#pragma endregion