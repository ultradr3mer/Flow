#include "CApp.h"
 
void CApp::OnLoop() {
	viewPort->Update();

	float m = viewPort->fwd.y;
	float b = viewPort->Position->y;
	float x = -b / m;

	*crosshair->Position = *viewPort->Position + viewPort->fwd * x;
	crosshair->Update();

	//mod->Rotation->y += 0.0001;
	for (int i = 0; i < objectCount; i++)
	{
		objects[i]->Update();
	}
}