#include "CApp.h"
 
void CApp::OnLoop() {
	viewPort->Update();

	float m = viewPort->fwd.y;
	float b = viewPort->Position->y;
	float x = -b / m;

	*crosshair->Position = *viewPort->Position + viewPort->fwd * x;
	crosshair->Update();

	//for (int i = 0; i < objectCount; i++)
	//{
	//	objects[i]->Update();
	//}

	ps->Update();

	enemySpawnTime -= 0.01f;

	spawner->Position = *viewPort->Position;

	if(enemySpawnTime <= 0)
	{
		int i;
		bool occupied = false;
		// find first free place
		for (i = 0; i < curEnemyCount; i++)
		{
			if(!enemys[i]->alive)
			{
				occupied = true;
				break;
			}
		}
		if(occupied)
			delete enemys[i];
		else
			curEnemyCount++;

		enemys[i] = new Enemy();

		enemySpawnTime = 1 / enemysPerSecond;
	}

	for (int i = 0; i < curEnemyCount; i++)
	{
		enemys[i]->Update();
	}

	for (int i = 0; i < curBombCount; i++)
	{
		bombs[i]->Update();
	}


	//if(resettingPos == 0)
	//{
	//	int tmp = 0;
	//	srand(SDL_GetTicks());
	//	int i = 0;
	//	while (i < ShipCount)
	//	{
	//		tmp = (int)round(linearRand(0.0f,1.0f)*100);
	//		for (int j = 0; j < i; j++)
	//		{
	//			if(shipPositions[j] == tmp)
	//			{
	//				tmp = -1;
	//				break;
	//			}
	//		}
	//		if(tmp != -1)
	//		{
	//			shipPositions[i++] = tmp;
	//		}
	//	}
	//}
	//if(resettingPos < 100)
	//{
	//	objects[resettingPos]->Status = GameTileTypeNone;
	//	resettingPos++;
	//}
}