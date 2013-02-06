#pragma once
#include "gamebase.h"
#include "ViewPort.h"
#include "Model.h"
#include "Scene.h"
#include "Tool.h"
#include "BrushTool.h"

class Player :
	public GameBase
{
private:
	Scene* scene;
	ListContainer<Tool> Tools;
	int curToolIndex;
public:
	vec3 RotationVec;
	vec3 MovingVec;
	Tool* CurTool;
	ViewPort* View;
	Player(Scene* scene);
	virtual ~Player(void);
	virtual void Update(void);
	void Click(uint btn);
	void ClickUp(uint btn);
};

