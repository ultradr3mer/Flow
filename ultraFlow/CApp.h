#ifndef _CAPP_H_
    #define _CAPP_H_

#include <stdlib.h>
#include <stdio.h>
#include <utility>
#include <iostream>
using namespace std;
/* Ensure we are using opengl's core profile only */
#define GL3_PROTOTYPES 1
//#include <gl3.h>

#include <GL/glew.h> 
#include <SDL.h>
#include "GenFunc.h"
#include "MeshData.h"
#include "ShaderData.h"
#include "DDSLoader.h"
#include "Texture.h"
#include "Model.h"
 
#define PROGRAM_NAME "Tutorial2"

class CApp {
    private:
        bool Running;
		long frame;
		int timebase;
		Model* mod;
		SDL_Surface* Surf_Display;

    public:
        CApp();
        int OnExecute(int argc, char **argv);

        bool OnInit(int argc, char **argv);
 
		void OnEvent(SDL_Event* Event);
 
        void OnLoop();
 
        void OnRender();
 
        void OnCleanup();

		static void updateError();
};
#endif
