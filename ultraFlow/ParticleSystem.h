#pragma once
#include "model.h"

#pragma region ParticleStruct
struct Particle
{
	vec3 Position;
	vec3 Vector;

	// remaining lifetime 
	float Life;
};
#pragma endregion

#pragma region ParticleAffectors
struct ParticleAffector
{
public:
	virtual void Affect(Particle*, int count);
};

struct ParticleAffectorBase : public ParticleAffector
{
public:
	float Lifetime;
	virtual void Affect(Particle*, int count);
};

struct ParticleAffectorSpawner : public ParticleAffector
{
public:
	virtual void Affect(Particle*, int count);
	vec3* Position;
};

struct ParticleAffectorGravity : public ParticleAffector
{
public:
	virtual void Affect(Particle*, int count);
	float Strength;
};
#pragma endregion

#pragma region ParticleSystem
const int particlecount = 2048;

class ParticleSystem :
	public Model
{
private:
	Particle particles[particlecount];
	ParticleAffector* affectors[64];
	int affectorCount;
public:
	ParticleSystem(void);
	~ParticleSystem(void);
	virtual void Draw();
	virtual void Update();
	void AppendAffector(ParticleAffector* affector);
};
#pragma endregion
