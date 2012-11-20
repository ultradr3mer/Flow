#include "CApp.h"
 
void CApp::OnLoop() {
	long newFrameTime = SDL_GetTicks();
	lastFrameDuration = newFrameTime - curFrameTime;
	curFrameTime = newFrameTime;

	viewPort->Update();

	dynamicsWorld->stepSimulation(lastFrameDuration/1000.0f, 7);

	//mod->Rotation->y += 0.0001;
	for (int i = 0; i < objectCount; i++)
	{
		objects[i]->Update();
	}
}