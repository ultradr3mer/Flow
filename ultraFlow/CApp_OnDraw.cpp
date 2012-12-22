#include "CApp.h"
void CApp::OnDraw() {
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glDepthMask(GL_TRUE); 
	glDepthFunc(GL_LEQUAL); 

	//Set current element
	curScene = scene;

	//Render Shadowbuffers
	scene->DrawShadowBuffers();

	//Activate View
	player->View->Bind();
	scene->EyePos = player->Position;
	scene->DrawScene(mainBufferSet);

	//Swap Buffers
	SDL_GL_SwapBuffers();

	//Show Fps
	frame++;
	int time= SDL_GetTicks(); 
	float fps = 0;
	if (time - timebase > 1000) {
		fps = frame*1000.0f/(time-timebase);
	 	timebase = time;
		frame = 0;
		printf("FPS: %f Visible: %f%%\n",fps,(float)DrawnObj / (DrawnObj + NotDrawnObj));
		DrawnObj = 0;
		NotDrawnObj = 0;
	}
}
