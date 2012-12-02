#include "CApp.h"

//* A simple function that prints a message, the error code returned by SDL,
// * and quits the application */
void sdldie(const char *msg)
{
    printf("%s: %s\n", msg, SDL_GetError());
    SDL_Quit();
    exit(1);
}
 
 
bool CApp::OnInit(int argc, char **argv) {
#pragma region sdl/opengl

	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,            8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,          8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,           8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,          8);
 
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,          16);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,            32);
 
    SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE,        8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE,    8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE,        8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE,    8);
 
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,  1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,  8);
 
    if((Surf_Display = SDL_SetVideoMode(screenX, screenY, 32, SDL_HWSURFACE | SDL_GL_DOUBLEBUFFER | SDL_OPENGL)) == NULL) {
        return false;
    }

	glewExperimental  = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
	  fprintf(stderr, "Error: %s\n", glewGetErrorString(err));

	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

#pragma endregion

//#pragma region bullet
//	//float m[16];
//	//body->getWorldTransform ().getOpenGLMatrix(m);
//	btBroadphaseInterface* broadphase = new btDbvtBroadphase();
//    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
//    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
//    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
//    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
//    dynamicsWorld->setGravity(btVector3(0,-10,0));
//	
// 
//    btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),1);   
//
//	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-1.5,0)));
//    btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
//    btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
//    dynamicsWorld->addRigidBody(groundRigidBody);
//#pragma endregion

	BodyGenerator::Init();

	GameTickLength = 1000 / 100;
	GameBaseTime = 0;

	viewPort = new ViewPort();
	//viewPort->Position->z = 2;
	viewPort->Position->y = 4;

	floor = new Model();
	floor->Mesh = MeshData::FromObj("floor.obj");
	floor->Shader = ShaderData::FromPlainText("simpleLight.vert","simpleLight.frag");
	floor->AppendTextureData(TextureData::FromDDS("floor.dds")->SetTarget(TexDiffuse));
	floor->AppendTextureData(TextureData::FromDDS("floor_n.dds")->SetTarget(TexNormal));
	//floor->Position->y = 1.0f;
	floor->Update();

	objectCount = 100;
	int size = (int)sqrt((float)objectCount);
	objects = new GameTile*[objectCount];
	GameTile* gametile;

	//TextureData* normal = TextureData::FromDDS("monkey_n.dds")->SetTarget(TexNormal);

	int posy, posx;
	for (int i = 0; i < objectCount; i++)
	{
		posy = i / size;
		posx = i - size * posy;

		gametile = new GameTile();

		gametile->Position.x = posx - size/2;
		gametile->Position.y = posy - size/2;
		//mod->Update();

		objects[i] = gametile;
	}

	MeshData* mesh = MeshData::FromObj("plane.obj");
	ShaderData* shader = ShaderData::FromPlainText("textureSimple.vert","textureSimple.frag");
	TextureData* texture = TextureData::FromDDS("crosshair.dds")->SetTarget(TexDiffuse);
	
	crosshair = new Model();
	crosshair->Mesh = mesh;
	crosshair->Shader = shader;
	crosshair->AppendTextureData(TextureData::FromDDS("crosshair.dds")->SetTarget(TexDiffuse));
	crosshair->Position->y = -0.01f;
	crosshair->Update();

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE,GL_ONE);

	SDL_WarpMouse(screenX/2,screenY/2);
	SDL_ShowCursor(0);

	hitCounter = 0;
	resettingPos = 0;

	return true;
}