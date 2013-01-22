#include "Player.h"
#include "GenFunc.h"
#include "BulletManager.h"


Player::Player(Scene* scene)
{
	View = new ViewPort();
	View->CalcReconstrucVecs = true;

	//Cursor = new Model();
	//scene->SceneDrawables.Add(Cursor);

	//Cursor->Mesh = MeshData::FromObj("cursor.obj");
	//Cursor->Material = MaterialData::FromXml("cursor.xmf");
	//Cursor->Position = vec3(0,1,0);

	pickConstraint = nullptr;

	this->scene = scene;
}

Player::~Player(void)
{
	delete View;
	//delete Cursor;
	GameObjList.Remove(this);
	if(pickConstraint != nullptr)
	{
		dynamicsWorld->removeConstraint(pickConstraint);
		delete pickConstraint;
	}
}

void Player::Update(void)
{
	float acceleration = 0.002f;
	float friction =  0.98f;
	float rotation = 0.7f;

	smoothMove = smoothMove * friction + (MovingVec.x * View->right + MovingVec.z * View->fwd) * acceleration;
	Position += smoothMove;

	smoothRotate = smoothRotate * rotation + Rotation * (1-rotation);

	View->Position = Position;
	View->Rotation = smoothRotate;

	scene->EyePos = View->Position;
	//scene->Update();

	if(pickConstraint != nullptr)
	{
		Cursor-> Position = Position + View->fwd*gOldPickingDist;
		pickConstraint->setPivotB(BulletVec3FromVec3(&Cursor-> Position));
	}
	else
	{	
		btVector3 from = BulletVec3FromVec3(&Position);
		btVector3 to = BulletVec3FromVec3(&(Position + View->fwd*100.0f));

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

void Player::Fire()
{
	btVector3 from = BulletVec3FromVec3(&Position);
	btVector3 to = BulletVec3FromVec3(&(Position + View->fwd*100.0f));
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

void Player::FireUp()
{
	if(pickConstraint != nullptr)
	{
		dynamicsWorld->removeConstraint(pickConstraint);
		delete pickConstraint;
		pickConstraint = nullptr;
	}
}