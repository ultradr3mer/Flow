#include "CApp.h"
 
void CApp::OnLoop() {
	viewPort->Update();

	//mod->Rotation->y += 0.0001;
	for (int i = 0; i < objectCount; i++)
	{
		objects[i]->Update();
	}
}