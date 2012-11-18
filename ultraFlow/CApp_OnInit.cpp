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

	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();

	viewPort = new ViewPort();
	viewPort->Position->z = 4;

	mod = new Model();
	mod->Mesh = MeshData::FromObj("monkey.obj");
	mod->Shader = ShaderData::FromPlainText("tutorial2.vert","tutorial2.frag");
	mod->AppendTextureData(TextureData::FromDDS("monkey.dds")->SetTarget(TexDiffuse));

	floor = new Model();
	floor->Mesh = MeshData::FromObj("floor.obj");
	floor->Shader = ShaderData::FromPlainText("tutorial2.vert","tutorial2.frag");
	floor->AppendTextureData(TextureData::FromDDS("floor.dds")->SetTarget(TexDiffuse));
	floor->Position->y = -1.2f;
	floor->Update();

	SDL_WarpMouse(screenX/2,screenY/2);

	return true;
}