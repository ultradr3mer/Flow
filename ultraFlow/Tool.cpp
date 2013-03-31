#include "Tool.h"

Tool::Tool(Scene* scene)
{
	pickConstraint = nullptr;

	Cursor = new Model();
	//scene->SceneDrawables.Add(Cursor);

	Cursor->Mesh = MeshData::FromObj("cursor.obj");
	Cursor->Material = MaterialData::FromXml("cursor.xmf");
	Cursor->Position = vec3(0,1,0);

	Acceleration = 0.002f;
	Friction =  0.98f;
	RotationBlur = 0.7f;

	this->scene = scene;

	equiped = false;
}

Tool::Tool()
{
	Cursor = new Model();

	pickConstraint = nullptr;

	Cursor->Mesh = MeshData::FromObj("cursor.obj");
	Cursor->Material = MaterialData::FromXml("cursor.xmf");
	Cursor->Position = vec3(0,1,0);

	Acceleration = 0.005f;
	Friction =  0.90f;
	RotationBlur = 0.5f;

	equiped = false;
}

Tool::~Tool(void)
{
	if(pickConstraint != nullptr)
	{
		dynamicsWorld->removeConstraint(pickConstraint);
		delete pickConstraint;
	}

	delete Cursor;
}

void Tool::Update()
{
	if(equiped)
	{
		PerformMovement();

		if(pickConstraint != nullptr)
		{
			Cursor-> Position = Position + PointingDir*gOldPickingDist;
			pickConstraint->setPivotB(BulletVec3FromVec3(&Cursor-> Position));
		}
		else
		{	
			btVector3 from = BulletVec3FromVec3(&Position);
			btVector3 to = BulletVec3FromVec3(&(Position + PointingDir*100.0f));

			btCollisionWorld::ClosestRayResultCallback	closestResults(from,to);
			//closestResults.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;
			dynamicsWorld->rayTest(from,to,closestResults);

			if (closestResults.hasHit())
			{
				Cursor->Position = Vec3FromBulletVec3(&from.lerp(to,closestResults.m_closestHitFraction));
				Cursor->Rotation = RotationFromNormal(Vec3FromBulletVec3(&closestResults.m_hitNormalWorld));
			}
		}
	}
}

void Tool::PerformMovement()
{
	SmoothMoveVec = SmoothMoveVec * Friction + MovingVec * Acceleration;
	Position += SmoothMoveVec;

	rotationTarget += RotationVec;
	RotationVec = vec3(0.0f);
	Rotation = Rotation * RotationBlur + rotationTarget * (1-RotationBlur);
}

void Tool::Click(uint btn)
{
	if(btn == SDL_BUTTON_LEFT)
	{
		btVector3 from = BulletVec3FromVec3(&Position);
		btVector3 to = BulletVec3FromVec3(&(Position + PointingDir*100.0f));
		btCollisionWorld::ClosestRayResultCallback	closestResults(from,to);
		//closestResults.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;
		dynamicsWorld->rayTest(from,to,closestResults);

		if (closestResults.hasHit())
		{
			btRigidBody* body = (btRigidBody*)btRigidBody::upcast(closestResults.m_collisionObject);
			if (body)
			{
				//other exclusions?
				if (!(body->isStaticObject() || body->isKinematicObject()))
				{
					pickedBody = body;
					pickedBody->setActivationState(DISABLE_DEACTIVATION);

					btVector3 pickPos = closestResults.m_hitPointWorld;
					btVector3 localPivot = body->getCenterOfMassTransform().inverse() * pickPos;

					btPoint2PointConstraint* p2p = new btPoint2PointConstraint(*body,localPivot);
					dynamicsWorld->addConstraint(p2p,true);
					pickConstraint = p2p;
					p2p->m_setting.m_tau = 0.001f;

					//save mouse position for dragging
					gOldPickingDist  = (pickPos-from).length();
				}
			}
		}
	}
}

void Tool::ClickUp(uint btn)
{
	if(btn == SDL_BUTTON_LEFT)
	{
		if(pickConstraint != nullptr)
		{
			dynamicsWorld->removeConstraint(pickConstraint);
			delete pickConstraint;
			pickConstraint = nullptr;
		}
	}
}

void Tool::Equip()
{
	ReinitMovement();
	scene->SceneDrawables.Add(Cursor);
	equiped = true;
}

void Tool::UnEquip()
{
	scene->SceneDrawables.Remove(Cursor);
	equiped = false;
}

void Tool::ReinitMovement()
{
	rotationTarget = Rotation;
}