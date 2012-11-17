#include "CApp.h"
 
void CApp::OnCleanup() {

	//md->~MeshData();
	//sd->~ShaderData();
	delete mod;

    SDL_Quit();
}