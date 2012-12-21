#include "CApp.h"
void CApp::OnDraw() {

	glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glDepthMask(GL_TRUE); 
	glDepthFunc(GL_LEQUAL); 

	//Sett current element
	curScene = scene;

	//Render Shadowbuffers
	scene->DrawShadowBuffers();

	//Activate View
	player->View->Bind();
	scene->EyePos = player->Position;
	DrawScene(mainBufferSet);

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
		printf("FPS: %f\n",fps);
	}
}

void CApp::DrawScene(BufferSet* bufferSet)
{
	CurrentBufferSet = bufferSet;

	//Draw deffered pass
	bufferSet->NormalPass.Bind();
	glEnable(GL_DEPTH_TEST); 
	scene->Draw(DrawingPassDeffered);

	//Draw Lights
	//bufferSet->OutBuffer->Bind();
	bufferSet->DefferedLightmap.Bind();
	scene->DrawDefferedLights();

	//Draw solid pass
	bufferSet->OutBuffer->Bind();
	scene->Draw(DrawingPassSolid);

	//Draw transparent pass
	glDisable(GL_DEPTH_TEST); 
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	scene->Draw(DrawingPassTransparent);

	//Output final image
	//bufferSet->OutBuffer->Bind();

	glDisable(GL_BLEND);
}