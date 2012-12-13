#include "CApp.h"

void CApp::OnEvent(SDL_Event* Event) {
	int x,y;
	//fire();

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
			viewPort->Rotation->y -= x / 1000.0f;
			viewPort->Rotation->x -= y / 1000.0f;
		}
		SDL_WarpMouse(screenX/2,screenY/2);
		break;
	case SDL_MOUSEBUTTONDOWN:
		if(Event->button.button == SDL_BUTTON_LEFT )
		{
			fire();
		}
		break;
	case SDL_KEYDOWN:
		switch(Event->key.keysym.sym){
		case SDLK_a:
			viewPort->movingVec.x += -1;
			break;
		case SDLK_d:
			viewPort->movingVec.x += +1;
			break;
		case SDLK_w:
			viewPort->movingVec.z += +1;
			break;
		case SDLK_s:
			viewPort->movingVec.z += -1;
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
			viewPort->movingVec.x -= -1;
			break;
		case SDLK_d:
			viewPort->movingVec.x -= +1;
			break;
		case SDLK_w:
			viewPort->movingVec.z -= +1;
			break;
		case SDLK_s:
			viewPort->movingVec.z -= -1;
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

void CApp::fire()
{
	int i;
	bool occupied = false;
	// find first free place
	for (i = 0; i < curBombCount; i++)
	{
		if(!bombs[i]->alive)
		{
			occupied = true;
			break;
		}
	}
	if(occupied)
		delete bombs[i];
	else
		curBombCount++;

	bombs[i] = new Bomb(crosshair->Position);
	
	//if(resettingPos < 100)
	//	return;

	//int hitx = (int)round(crosshair->Position->x / 2);
	//int hity = (int)round(crosshair->Position->z / 2);

	//int comparePos[10];
	//int compareWriter = 0;

	//for (int i = 0; i < objectCount; i++)
	//{
	//	// hit tile found ?
	//	if(objects[i]->Position.x == hitx &&
	//		objects[i]->Position.y == hity)
	//	{
	//		// check if not flipped
	//		if(objects[i]->Status == GameTileTypeNone)
	//		{
	//			// check if Ship is placed there
	//			for (int j = 0; j < ShipCount; j++)
	//			{
	//				if(i == shipPositions[j])
	//				{
	//					// ship found
	//					objects[i]->Status = GameTileTypeCross;
	//					hitCounter++;
	//					break;
	//				}
	//			}
	//			// show water
	//			if(objects[i]->Status == GameTileTypeNone)
	//				objects[i]->Status = GameTileTypeWater;

	//			break;
	//		}
	//	}

	//	if(objects[i]->Status == GameTileTypeCross)
	//		comparePos[compareWriter++] = i;
	//}

	//if(hitCounter == ShipCount)
	//{
	//	resettingPos = 0;
	//	hitCounter = 0;
	//}
}