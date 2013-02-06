#include "Player.h"
#include "GenFunc.h"
#include "BulletManager.h"

Player::Player(Scene* scene)
{
	View = new ViewPort();
	View->CalcReconstrucVecs = true;
	Tools.Add(new BrushTool(scene));
	Tools.Add(new Tool(scene));
	CurTool = Tools.Get(0);
	CurTool->Equip();

	this->scene = scene;
}

Player::~Player(void)
{
	delete View;
	//delete CurTool;
	//GameObjList.Remove(this);
}

void Player::Update(void)
{
	CurTool->MovingVec = MovingVec.x * View->right + MovingVec.z * View->fwd;
	CurTool->PointingDir = View->fwd;
	CurTool->RotationVec += RotationVec;
	RotationVec = vec3(0.0f);

	Position = CurTool->Position;
	scene->EyePos = Position;

	View->Position = Position;
	View->Rotation = CurTool->Rotation;
}

void Player::Click(uint btn)
{
	if(btn == SDL_BUTTON_WHEELUP || btn == SDL_BUTTON_WHEELDOWN)
	{
		if(btn == SDL_BUTTON_WHEELDOWN)
			curToolIndex--;
		else if(btn == SDL_BUTTON_WHEELUP)
			curToolIndex++;

		if(curToolIndex < 0)
			curToolIndex = Tools.Length - 1;
		else if(curToolIndex >= Tools.Length)
			curToolIndex = 0;

		Tool* newTool = Tools.Get(curToolIndex);
		newTool->Position = CurTool->Position;
		newTool->Rotation = CurTool->Rotation;
		newTool->SmoothMoveVec = CurTool->SmoothMoveVec;

		CurTool->UnEquip();
		newTool->Equip();

		CurTool = newTool;
	}
	else
	{
		CurTool->Click(btn);
	}
}

void Player::ClickUp(uint btn)
{
	CurTool->ClickUp(btn);
}