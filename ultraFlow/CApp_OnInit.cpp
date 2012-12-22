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

	//FILE * pFile = fopen(FullFileName("","random_vectors.txt"),"w");
	//for (int i = 0; i < 64; i++)
	//{
	//	vec3 rnd = sphericalRand(1.0f);

	//	fprintf(pFile, "vec3(%f, %f, %f),\n",rnd.x,rnd.y,rnd.z);
	//}
	//fclose(pFile);

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

	glDisable( GL_LINE_SMOOTH );
    glDisable( GL_POLYGON_SMOOTH );
    glDisable( GL_MULTISAMPLE );

	//check whether extension string can be found
	if (!strstr((char*)glGetString(GL_EXTENSIONS), 
		"GL_EXT_texture_filter_anisotropic"))
	{
		printf("No Anisotropic filtering");
	}
	#pragma endregion

	scene = new Scene();

	BulletManager::Init();
	object = nullptr;

	// Initialize Renderbuffers
	mainBufferSet = new BufferSet(screenX,screenY);

	// Initialize game stepping
	GameTickLength = 1000 / 100;
	GameBaseTime = 0;

	// Setup scene
	Model* floor = new Model();
	floor->Mesh = MeshData::FromObj("floor.obj");
	floor->Material = MaterialData::FromXml("floor.xmf");
	scene->SceneDrawables.Add(floor);

	// Create player
	player = new Player(scene);
	player->Position.z = 2;
	player->Position.y = 4;

	PhysicsModel* mod;

	for (int i = 0; i < 20; i++)
	{
		mod = new PhysicsModel();
		mod->Material = MaterialData::FromXml("sae_shuttle.xmf");
		mod->Body = BulletManager::FromObj("sae_shuttle_pbox.obj");
		mod->Body->getWorldTransform().setOrigin(btVector3(0.0f,i+1.0f,0.0f));
		mod->Mesh = MeshData::FromObj("sae_shuttle.obj");
		scene->SceneDrawables.Add(mod);
	}

	SDL_WarpMouse(screenX/2,screenY/2);
	SDL_ShowCursor(0);

	return true;
}