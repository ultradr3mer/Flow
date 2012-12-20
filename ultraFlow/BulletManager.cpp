#include "BulletManager.h"
#include "ObjLoader.h"
#include "MeshData.h"

btDiscreteDynamicsWorld* dynamicsWorld;

const int cacheSize = 512;
int writerPos;
btCollisionShape* shapeCache[cacheSize];
char shapeCacheNames[cacheSize][32];
long curFrameTime;
int lastFrameDuration;

void BulletManager::Init(void)
{
	btBroadphaseInterface* broadphase = new btDbvtBroadphase();
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0,-10,0));

    btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),1);   

	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-1,0)));
    btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
    btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
    dynamicsWorld->addRigidBody(groundRigidBody);

	for (int i = 0; i < cacheSize; i++)
	{
		shapeCache[i] = nullptr;
	}
}

btRigidBody* BulletManager::FromObj(char* source)
{
	btCollisionShape* fallShape = ShapeFromObj(source);
	//fallShape->setLocalScaling(btVector3(0.5f,0.5f,0.5f));
	//float scale = 2.0f/3.0f;
	//fallShape->setLocalScaling(btVector3(scale,scale,scale));

	//btCollisionShape* fallShape = new btSphereShape(0.01f);

	btDefaultMotionState* fallMotionState =
		new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)));
    btScalar mass = 1;
    btVector3 fallInertia(0,0,0);
    fallShape->calculateLocalInertia(mass,fallInertia);
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,fallShape,fallInertia);

    btRigidBody* fallRigidBody = new btRigidBody(fallRigidBodyCI);
    dynamicsWorld->addRigidBody(fallRigidBody);

	return fallRigidBody;
}

btCollisionShape* BulletManager::ShapeFromObj(char* source)
{
	for (int i = 0; i < cacheSize && shapeCache[i] != nullptr ; i++)
	{
		if(strcmp(source,shapeCacheNames[i]) == 0)
			return shapeCache[i];
	}

	ObjLoader::Load(source);
	for (int i = 0; i < dataLenght; i++)
	{
		positions[i] = btVector3(positionBuffer[i][0],positionBuffer[i][1],positionBuffer[i][2]);
	}
	btCollisionShape* fallShape = new btConvexHullShape(*positions,dataLenght,sizeof(positions[0]));
	MeshData::Clear();

	if(writerPos >= cacheSize)
		writerPos = 0;

	strcpy(shapeCacheNames[writerPos],source);
	shapeCache[writerPos] = fallShape;
	writerPos++;

	return fallShape;
}

void BulletManager::Step()
{
	long newFrameTime = SDL_GetTicks();
	lastFrameDuration = newFrameTime - curFrameTime;
	curFrameTime = newFrameTime;

	if(lastFrameDuration)
		dynamicsWorld->stepSimulation(lastFrameDuration / 1000.0f, 7);
}