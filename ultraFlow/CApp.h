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
#include "BodyGenerator.h"
//#include "GameTile.h"
#include "Enemy.h"
#include "ParticleSystem.h"
#include "Bomb.h"
 

class CApp {
    private:
        bool Running;
		long frame;
		int timebase;
		Bomb* bombs[256];
		int curBombCount;
		Model* crosshair;
		int objectCount;
		Model* floor;
		ViewPort* viewPort;
		SDL_Surface* Surf_Display;
		long GameBaseTime;
		uint GameTickLength;
		int hitCounter;
		int resettingPos;
		ParticleSystem* ps;
		ParticleAffectorSpawner* spawner;

    public:
        CApp();
        int OnExecute(int argc, char **argv);

        bool OnInit(int argc, char **argv);
 
		void OnEvent(SDL_Event* Event);
 
        void OnLoop();
 
        void OnRender();
 
        void OnCleanup();

		void fire();
};
#endif
