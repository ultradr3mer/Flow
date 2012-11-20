#ifndef _CAPP_H_
    #define _CAPP_H_

#include "Base.h"
#include "GenFunc.h"
#include "MeshData.h"
#include "ShaderData.h"
#include "DDSLoader.h"
#include "TextureData.h"
#include "Model.h"
#include "MatrixGenerator.h"
#include "ViewPort.h"
#include "GameBase.h"
#include "PhysicsModel.h"
 
#define PROGRAM_NAME "Tutorial2"

class CApp {
    private:
        bool Running;
		long frame;
		int timebase;
		long curFrameTime;
		int lastFrameDuration;
		btDiscreteDynamicsWorld* dynamicsWorld;
		PhysicsModel* mod;
		Model* floor;
		ViewPort* viewPort;
		SDL_Surface* Surf_Display;

    public:
        CApp();
        int OnExecute(int argc, char **argv);

        bool OnInit(int argc, char **argv);
 
		void OnEvent(SDL_Event* Event);
 
        void OnLoop();
 
        void OnRender();
 
        void OnCleanup();
};
#endif
