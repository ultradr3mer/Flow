#include "CApp.h"

void CApp::OnEvent(SDL_Event* Event) {
	int x,y;
	switch (Event->type)
	{
	case SDL_QUIT:
		Running = false;
		break;
	case SDL_MOUSEMOTION:
		x = Event->motion.x-screenX/2;
		y = Event->motion.y-screenY/2;
		if(x != 0 || y != 0)
		{
			player->Rotation.y -= x / 1000.0f;
			player->Rotation.x -= y / 1000.0f;
		}
		SDL_WarpMouse(screenX/2,screenY/2);
		break;
	case SDL_MOUSEBUTTONDOWN:
		if(Event->button.button == SDL_BUTTON_LEFT )
		{
			player->Fire();
		}
		break;
	case SDL_MOUSEBUTTONUP:
		if(Event->button.button == SDL_BUTTON_LEFT )
		{
			player->FireUp();
		}
		break;
	case SDL_KEYDOWN:
		switch(Event->key.keysym.sym){
		case SDLK_a:
			player->MovingVec.x += -1;
			break;
		case SDLK_d:
			player->MovingVec.x += +1;
			break;
		case SDLK_w:
			player->MovingVec.z += +1;
			break;
		case SDLK_s:
			player->MovingVec.z += -1;
			break;
		case SDLK_ESCAPE:
			Running = false;
			break;
		default:
			break;
		}
		break;
	case SDL_KEYUP:
		switch(Event->key.keysym.sym){
		case SDLK_a:
			player->MovingVec.x -= -1;
			break;
		case SDLK_d:
			player->MovingVec.x -= +1;
			break;
		case SDLK_w:
			player->MovingVec.z -= +1;
			break;
		case SDLK_s:
			player->MovingVec.z -= -1;
			break;
		case SDLK_ESCAPE:
			Running = false;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}