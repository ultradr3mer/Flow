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

	GridRes = 4.0f;
}

BrushTool::~BrushTool(void)
{
}

void BrushTool::Update()
{
	if(equiped)
	{
		PerformMovement();

		vec3 from = Position;
		vec3 to = Position + PointingDir*100.0f;

		btVector3 btFrom = BulletVec3FromVec3(&from);
		btVector3 btTo = BulletVec3FromVec3(&to);
		btCollisionWorld::ClosestRayResultCallback	closestResults(btFrom,btTo);
		Ray ray;

		switch (placeMode)
		{
		case PlaceModeNone:
			//closestResults.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;
			dynamicsWorld->rayTest(btFrom,btTo,closestResults);

			if (closestResults.hasHit())
			{
				hitPos = Vec3FromBulletVec3(&btFrom.lerp(btTo,closestResults.m_closestHitFraction));
				hitNorm = Vec3FromBulletVec3(&closestResults.m_hitNormalWorld);

				// Snapping
				hitPos = round(hitPos*GridRes)/GridRes;
				hitNorm = round(hitNorm);

				Cursor->Position = hitPos;
				Cursor->Rotation = RotationFromNormal(brushNormal = hitNorm);
			}
			break;
		case PlaceModeSizeXY:
			ray = Ray(from,to);
			pointingHelper.RayTest(&ray);

			hitPos = ray.HitPos();

			// Snapping
			hitPos = round(hitPos*GridRes)/GridRes;

			brushDist = length(hitPos - Position);
			brushSizeXy = hitPos - brushPosition;
			brushSize = brushSizeXy;
			break;
		case PlaceModeSizeZ:
			ray = Ray(from,to);
			pointingHelper.RayTest(&ray);

			// Snapping
			ray.HitCoords.x = round(ray.HitCoords.x*GridRes)/GridRes;

			brushSize = brushSizeXy + brushNormal * ray.HitCoords.x;
			break;
		default:
			break;
		}

		//Make Size always Positive
		if(ghostVisible)
		{
			// Snapping
			brushSize = round(brushSize*GridRes)/GridRes;

			positiveSize = brushSize;
			positiveSize *= sign(positiveSize);
			positivePosition = brushPosition - (positiveSize - brushSize) * 0.5f;
			ghost->Position = positivePosition;
		}
	}
}

void BrushTool::Click(uint btn)
{
	vec3 plnPoints[3];
	mat4 tmpMat;

	switch (placeMode)
	{
	case PlaceModeNone:
		placeMode = PlaceModeSizeXY;

		brushPosition = hitPos;
		//ghost->Position = brushPosition;

		tmpMat = MatrixFromPosAng(hitPos,Cursor->Rotation);
		plnPoints[0] = hitPos;
		plnPoints[1] = vec3(tmpMat * vec4(1,0,0,1));
		plnPoints[2] = vec3(tmpMat * vec4(0,0,1,1));
		pointingHelper = TraceableTriangle(plnPoints);

		scene->SceneDrawables.Add(ghost);
		scene->SceneDrawables.Remove(Cursor);
		ghostVisible = true;
		break;
	case PlaceModeSizeXY:
		if(((int)(brushSizeXy.x != 0) +
			(int)(brushSizeXy.y != 0) +
			(int)(brushSizeXy.z != 0)) > 1)
		{
			placeMode = PlaceModeSizeZ;

			plnPoints[0] = hitPos;
			plnPoints[1] = hitPos+hitNorm;
			plnPoints[2] = hitPos+normalize(cross(PointingDir,hitNorm));
			pointingHelper = TraceableTriangle(plnPoints);
		}
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