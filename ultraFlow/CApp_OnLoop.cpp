#include "CApp.h"
 
void CApp::OnLoop() {
	viewPort->Update();

	float m = viewPort->fwd.y;
	float b = viewPort->Position->y;
	float x = -b / m;

	*crosshair->Position = *viewPort->Position + viewPort->fwd * x;
	crosshair->Update();

	for (int i = 0; i < objectCount; i++)
	{
		objects[i]->Update();
	}

	if(resettingPos == 0)
	{
		int tmp = 0;
		srand(SDL_GetTicks());
		int i = 0;
		while (i < ShipCount)
		{
			tmp = round(linearRand(0.0f,1.0f)*100);
			for (int j = 0; j < i; j++)
			{
				if(shipPositions[j] == tmp)
				{
					tmp = -1;
					break;
				}
			}
			if(tmp != -1)
			{
				shipPositions[i++] = tmp;
			}
		}
	}
	if(resettingPos < 100)
	{
		objects[resettingPos]->Status = GameTileTypeNone;
		resettingPos++;
	}
}