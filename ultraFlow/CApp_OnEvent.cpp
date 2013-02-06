#include "CApp.h"

float lastMouseFire;
float lastMouseFireUp;

void CApp::OnEvent(SDL_Event* Event) {
	int x,y;
	switch (Event->type)
	{
	case SDL_QUIT:
		Running = false;
		break;
	case SDL_MOUSEMOTION:
		x = screenX/2-Event->motion.x;
		y = screenY/2-Event->motion.y;
		if(x != 0 || y != 0)
		{
			player->RotationVec.y += x / 1000.0f;
			player->RotationVec.x += y / 1000.0f;
		}
		SDL_WarpMouse(screenX/2,screenY/2);
		break;
	case SDL_MOUSEBUTTONDOWN:
		if(lastMouseFire  + 0.05 < gameTime)
		{
			player->Click(Event->button.button);
			lastMouseFire = gameTime;
		}
		break;
	case SDL_MOUSEBUTTONUP:
		if(lastMouseFireUp  + 0.05 < gameTime)
		{
			player->ClickUp(Event->button.button);
			lastMouseFireUp = gameTime;
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