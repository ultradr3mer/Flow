#pragma once
#include "tool.h"
#include "ShaderData.h"

enum PlaceMode
{
	PlaceModeNone,
	PlaceModeSizeXY,
	PlaceModeSizeZ
};

class BrushTool :
	public Tool
{
private:
	Model* ghost;
	bool ghostVisible;

	enum PlaceMode placeMode;
	Scene* scene;
	vec3 brushPosition;
	vec3 brushSizeXy;
	vec3 brushSize;
	vec3 brushNormal;

	vec3 positiveSize;
	vec3 positivePosition;

	float brushDist;
	float xRot;
public:
	BrushTool(Scene* scene);
	virtual ~BrushTool(void);
	virtual void Update();
	virtual void Click(uint btn);
	virtual void ClickUp(uint btn);

	virtual void Equip();
	virtual void UnEquip();
};

