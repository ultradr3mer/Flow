#include "CApp.h"
 
void CApp::OnCleanup() {

	//md->~MeshData();
	//sd->~ShaderData();
	for (int i = 0; i < curEnemyCount; i++)
	{
		delete enemys[i];
	}

    SDL_Quit();
}