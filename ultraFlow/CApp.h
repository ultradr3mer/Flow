#ifndef _CAPP_H_
    #define _CAPP_H_

#include "Base.h"
#include "GenFunc.h"
#include "MeshData.h"
#include "ShaderData.h"
#include "DDSLoader.h"
#include "TextureData.h"
#include "Model.h"
#include "ViewPort.h"
#include "GameBase.h"
#include "PhysicsModel.h"
#include "BulletManager.h"
#include "Player.h"
#include "XmlParser.h"
#include "FrameBufferData.h"
#include "Filter2D.h"
#include "Sun.h"
#include "Scene.h"
#include "TraceableTriangle.h"
#include "PlnEquation.h"
#include "Ray.h"
#include "Line2d.h"
 
class CApp {
    private:
        bool Running;
		long frame;
		int timebase;
		float gameTime;
		Player* player;
		SDL_Surface* Surf_Display;
		Uint32 GameBaseTime;
		uint GameTickLength;
		GameBase* object;
		BufferSet* mainBufferSet;
		Scene* scene;

    public:
        CApp();
        int OnExecute(int argc, char **argv);

        bool OnInit(int argc, char **argv);
 
		void OnEvent(SDL_Event* Event);
 
        void OnUpdate();
 
        void OnDraw();
 
        void OnCleanup();
};
#endif
