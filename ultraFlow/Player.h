#pragma once
#include "gamebase.h"
#include "ViewPort.h"
#include "Model.h"
#include "Scene.h"

class Player :
	public GameBase
{
private:
	Model* Cursor;
	vec3 smoothMove;
	vec3 smoothRotate;
	btRigidBody* pickedBody;
	btPoint2PointConstraint* pickConstraint;
	float gOldPickingDist;
public:
	ViewPort* View;
	vec3 MovingVec;
	Player(Scene* scene);
	virtual ~Player(void);
	virtual void Update(void);
	void Fire();
	void FireUp();
};

