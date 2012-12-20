#include "CApp.h"
 
void CApp::OnCleanup() 
{
	GameObjList.ClearDelete();

	delete mainBufferSet;

    SDL_Quit();
}