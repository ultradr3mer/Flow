#include "CApp.h"
 
void CApp::OnUpdate() {
	//mod->Rotation->y += 0.0001;
	//for (int i = 0; i < objectCount; i++)
	//{
	//	objects[i]->Update();
	//}
	GameObjList.InitReader(&object);
	while (GameObjList.Read())
	{
		object->Update();
	}
}