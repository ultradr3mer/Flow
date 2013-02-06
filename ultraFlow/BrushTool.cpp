#include "BrushTool.h"

BrushTool::BrushTool(Scene* scene)
{
	//scene->SceneDrawables.Add(Cursor);

	ghost = new Model();
	ghost->Material = MaterialData::FromXml("brushGhost.xmf");
	ghost->Mesh = MeshData::FromObj("brush.obj");
	ghost->Mesh->maxRadius = FLT_MAX;
	ghost->UniformInserts.Add(new UniformInsert(VecSize,DataTypeVector3,&positiveSize));
	//scene->SceneDrawables.Add(Ghost);
	ghostVisible = false;

	placeMode = PlaceModeNone;

	this->scene = scene;
}

BrushTool::~BrushTool(void)
{
}

void BrushTool::Update()
{
	if(equiped)
	{
		PerformMovement();

		btVector3 from = BulletVec3FromVec3(&Position);
		btVector3 to = BulletVec3FromVec3(&(Position + PointingDir*100.0f));

		btCollisionWorld::ClosestRayResultCallback	closestResults(from,to);
		//closestResults.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;
		dynamicsWorld->rayTest(from,to,closestResults);

		switch (placeMode)
		{
		case PlaceModeNone:
			if (closestResults.hasHit())
			{
				Cursor->Position = Vec3FromBulletVec3(&from.lerp(to,closestResults.m_closestHitFraction));
				Cursor->Rotation = RotationFromNormal(Vec3FromBulletVec3(&closestResults.m_hitNormalWorld));

				// Snapping
				Cursor->Position = round(Cursor->Position*2.0f)/2.0f;
				Cursor->Rotation = round(Cursor->Rotation*4.0f)/4.0f;
			}
			break;
		case PlaceModeSizeXY:
			if (closestResults.hasHit())
			{
				brushNormal = Vec3FromBulletVec3(&closestResults.m_hitNormalWorld);

				vec3 hitPos = Vec3FromBulletVec3(&from.lerp(to,closestResults.m_closestHitFraction));
				brushDist = length(hitPos - Position);
				brushSizeXy = hitPos - brushPosition;
				brushSize = brushSizeXy;
			}
			break;
		case PlaceModeSizeZ:
			brushSize = brushSizeXy + brushNormal * (Rotation.x - xRot) * brushDist * 6.0f;
			break;
		default:
			break;
		}

		//Make Size always Positive
		if(ghostVisible)
		{
			// Snapping
			brushSize = round(brushSize*2.0f)/2.0f;

			positiveSize = brushSize;
			positiveSize *= sign(positiveSize);
			positivePosition = brushPosition - (positiveSize - brushSize) * 0.5f;
			ghost->Position = positivePosition;
		}
	}
}

void BrushTool::Click(uint btn)
{
	switch (placeMode)
	{
	case PlaceModeNone:
		placeMode = PlaceModeSizeXY;

		brushPosition = Cursor->Position;
		//ghost->Position = brushPosition;

		scene->SceneDrawables.Add(ghost);
		scene->SceneDrawables.Remove(Cursor);
		ghostVisible = true;
		break;
	case PlaceModeSizeXY:
		placeMode = PlaceModeSizeZ;

		xRot = Rotation.x;
		break;
	case PlaceModeSizeZ:
		placeMode = PlaceModeNone;

		scene->Map->AddBrush(new Brush(positivePosition,positiveSize,0,false));

		scene->SceneDrawables.Add(Cursor);
		scene->SceneDrawables.Remove(ghost);
		ghostVisible = false;
		break;
	default:
		break;
	}
}

void BrushTool::ClickUp(uint btn)
{
}

void BrushTool::Equip()
{
	ReinitMovement();

	if(ghostVisible)
		scene->SceneDrawables.Add(ghost);
	else
		scene->SceneDrawables.Add(Cursor);

	equiped = true;
}

void BrushTool::UnEquip()
{
	if(ghostVisible)
		scene->SceneDrawables.Remove(ghost);
	else
		scene->SceneDrawables.Remove(Cursor);

	equiped = false;
}