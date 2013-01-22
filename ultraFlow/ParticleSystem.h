#pragma once
#include "model.h"

#pragma region ParticleStruct
struct Particle
{
	vec3 Position;
	vec3 Vector;
	float Alpha;

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

enum AlphaFunc
{
	AlphaFuncFadeOut,
	AlphaFuncFadeIn,
	AlphaFuncFadeInOut
};

struct ParticleAffectorBase : public ParticleAffector
{
public:
	enum AlphaFunc AlphaFunc;
	float Lifetime;
	virtual void Affect(Particle*, int count);
};

struct ParticleAffectorSpawner : public ParticleAffector
{
public:
	ParticleAffectorSpawner();
	vec3 spawnSize;
	int particlePerSecond;
	float particlesToSpawn;
	virtual void Affect(Particle*, int count);
	vec3 Position;
	vec3 InitialVec;
	vec3 InitialVecRandom;
};

struct ParticleAffectorGravity : public ParticleAffector
{
public:
	virtual void Affect(Particle*, int count);
	float Strength;
};
#pragma endregion

#pragma region ParticleSystem
class ParticleSystem :
	public Model
{
private:
	Particle* particles;
	ParticleAffector* affectors[64];
	int affectorCount;
	int maxParticles;

	//ParticleData
	GLfloat* particlePositions;
	GLfloat* particleAlpha;
	GLint curParticleCount;
	GLuint vbos[2];
public:
	ParticleAffectorBase* BaseAffector;
	ParticleSystem(int MaxParticles);
	~ParticleSystem(void);
	virtual void Draw(enum DrawingPass pass);
	virtual void Update();
	GLfloat ParticleSize;
	void AppendAffector(ParticleAffector* affector);
};
#pragma endregion
