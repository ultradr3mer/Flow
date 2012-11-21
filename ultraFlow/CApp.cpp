#include "CApp.h"

char execPath[512];
int execPathLenght;
void preparePath(char*);
bool useVertexArrays = false;

CApp::CApp() {
    Running = true;
}
 
int CApp::OnExecute(int argc, char **argv) {
	preparePath(argv[0]);

	if(OnInit(argc, argv) == false) {
		return -1;
	}


	SDL_Event Event;
    while(Running) {
		while(SDL_PollEvent(&Event)) 
		{
			OnEvent(&Event);
		}
		while ((SDL_GetTicks() - GameBaseTime) > GameTickLength)
		{
			GameBaseTime += GameTickLength;
			OnLoop();
		}

		BodyGenerator::Step();

        OnRender();
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

char* FullFileName(char* file)
{
	execPath[execPathLenght] = '\0';
	return strcat(execPath,file);
}
 
int main(int argc, char **argv) {
    CApp theApp;
 
    return theApp.OnExecute(argc, argv);
}
