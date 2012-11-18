#include "CApp.h"
 
void CApp::OnLoop() {
	viewPort->Update();

	//mod->Rotation->y += 0.0001;
	mod->Update();
}