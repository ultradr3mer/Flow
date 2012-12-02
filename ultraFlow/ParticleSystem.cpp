#include "ParticleSystem.h"
#include "ViewPort.h"

#pragma region ParticleAffectors
void ParticleAffector::Affect(Particle* particles, int count)
{
}

void ParticleAffectorBase::Affect(Particle* particles, int count)
{
	float step = 1 / Lifetime / 100;
	for (int i = 0; i < count; i++)
	{
		particles[i].Position += particles[i].Vector;
		particles[i].Life -= step;
	}
}

void ParticleAffectorSpawner::Affect(Particle* particles, int count)
{
	for (int i = 0; i < count; i++)
	{
		if(particles[i].Life <= 0)
		{
			particles[i].Life = 1;
			particles[i].Position = gaussRand(*Position, vec3(2.0f));
			particles[i].Vector = vec3(0);
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
ParticleSystem::ParticleSystem(void)
{
	Mesh = MeshData::FromObj("plane.obj");
	Shader = ShaderData::FromPlainText("Particle.vert","Particle.frag");
	for (int i = 0; i < particlecount; i++)
	{
		particles[i].Position = gaussRand(vec3(0), vec3(2.5f));
		particles[i].Life = linearRand(0.0f,1.0f);
	}
	affectorCount = 0;

	ParticleAffectorBase* base = new ParticleAffectorBase();
	base->Lifetime = 4;
	AppendAffector(base);
}

ParticleSystem::~ParticleSystem(void)
{
	delete[] affectors;
}

void ParticleSystem::AppendAffector(ParticleAffector* affector)
{
	affectors[affectorCount++] = affector;
}

void ParticleSystem::Draw()
{
	Mesh->Bind();
	Shader->Bind();
	for (int i = 0; i < texCount; i++)
	{
		Textures[i]->Bind();
	}

	ShaderData::UniformMatrix4fv(MatViewProjection, curViewPort->ViewProjectionMatrix);
	ShaderData::UniformMatrix4fv(MatModelView, mat4(1.0f));
	ShaderData::Uniform1f(FltAspect, curViewPort->Aspect);
	ShaderData::Uniform1f(FltSize, 0.02f);
	//ShaderData::Uniform3fv(VecOrigin, *Position);

	float alpha = 0;
	for (int i = 0; i < particlecount; i++)
	{
		if(particles[i].Life > 0)
		{
			alpha = particles[i].Life - 0.5f;
			alpha *= sign(alpha);
			alpha = 1 - alpha * 2;
			ShaderData::Uniform1f(FltAlpha, alpha);
			ShaderData::Uniform3fv(VecOrigin, particles[i].Position);
			glDrawElements(GL_TRIANGLES, Mesh->Length, GL_UNSIGNED_INT, 0);
		}
	}
}

void ParticleSystem::Update()
{
	for (int i = 0; i < affectorCount; i++)
	{
		affectors[i]->Affect(particles,particlecount);
	}
}
#pragma endregion