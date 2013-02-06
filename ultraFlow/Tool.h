#pragma once
#include "gamebase.h"
#include "Scene.h"
#include "BulletManager.h"

class Tool :
	public GameBase
{
protected:
	btRigidBody* pickedBody;
	btPoint2PointConstraint* pickConstraint;
	float gOldPickingDist;
	Model* Cursor;
	vec3 rotationTarget;
	Scene* scene;
	bool equiped;
public:
	float Acceleration;
	float Friction;
	float RotationBlur;

	vec3 SmoothMoveVec;

	vec3 PointingDir;
	vec3 MovingVec;
	vec3 RotationVec;

	Tool();
	Tool(Scene* scene);
	virtual ~Tool(void);

	virtual void Update();
	virtual void Click(uint btn);
	virtual void ClickUp(uint btn);
	virtual void Equip();
	virtual void UnEquip();
	void PerformMovement();
	void ReinitMovement();
};

