#pragma once
#include "particlesystem.h"

struct ParticleAffector
{
public:
	virtual void Affect(Particle*);
};

struct Mover : public ParticleAffector
{
public:
	virtual void Affect(Particle* particles)
	{
		int count = sizeof(particles)/sizeof(particles[0]);

		for (int i = 0; i < count; i++)
		{
			particles[i].Position += particles[i].Vector;
		}
	}
};