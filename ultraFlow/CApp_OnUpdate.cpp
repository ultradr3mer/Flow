#include "CApp.h"
 
void CApp::OnUpdate() {

	GameObjList.InitReader();
	while (GameObjList.Read())
	{
		GameObjList.Cur->Update();
	}
}