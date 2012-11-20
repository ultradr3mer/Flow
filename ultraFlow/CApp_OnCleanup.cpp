#include "CApp.h"
 
void CApp::OnCleanup() {

	//md->~MeshData();
	//sd->~ShaderData();
	delete[] objects;

    SDL_Quit();
}