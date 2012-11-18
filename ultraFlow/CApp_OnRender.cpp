#include "CApp.h"
void CApp::OnRender() {

	glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST); 
	glDepthMask(GL_TRUE); 
	glDepthFunc(GL_LEQUAL); 

	viewPort->Bind();
	mod->Draw();
	floor->Draw();

	SDL_GL_SwapBuffers();

	//frame++;
	//int time=glutGet(GLUT_ELAPSED_TIME);
	//float fps = 0;
	//if (time - timebase > 1000) {
	//	fps = frame*1000.0f/(time-timebase);
	// 	timebase = time;
	//	frame = 0;
	//	printf("%f\n",fps);
	//}

    /* Cleanup all the things we bound and allocated */
	//delete md;
	//delete sd;
   
    //glDisableVertexAttribArray(0);
    //glDisableVertexAttribArray(1);
    //glDeleteBuffers(3, vbo);
    //glDeleteVertexArrays(1, &vao);
}