#include "CApp.h"

bool useVertexArrays = false;

CApp::CApp() 
{
    Running = true;
}
 
int CApp::OnExecute(int argc, char **argv) {
	if(OnInit(argc, argv) == false) {
		return -1;
	}

	SDL_Event Event;
	bool calculatedFrame;
    while(Running) {
		//BulletManager::Step();

		while(SDL_PollEvent(&Event)) 
		{
			OnEvent(&Event);
		}
		calculatedFrame= false;
		while ((SDL_GetTicks() - GameBaseTime) > GameTickLength)
		{
			GameBaseTime += GameTickLength;
			OnUpdate();
			calculatedFrame = true;
		}

		BulletManager::Step();

		OnDraw();

    }
 
    OnCleanup();
 
    return 0;
}

void preparePath(char* rawpath)
{
	int length = 0;
	//int i;
	char* tmpPointer;
	tmpPointer = rawpath;
	while (tmpPointer[length]!='\0')
	{
		length++;
	}
	while (tmpPointer[length]!='\\')
	{
		length--;
	}
	execPathLenght = length+1;
	for (int i = 0; i < execPathLenght; i++)
	{
		execPath[i] = tmpPointer[i];
	}
}
 
int main(int argc, char **argv) {
    CApp theApp;
 
	preparePath(argv[0]);
    return theApp.OnExecute(argc, argv);
}
