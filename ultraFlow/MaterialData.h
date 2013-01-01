#pragma once
#include "XmlParser.h"
#include "ShaderData.h"
#include "TextureData.h"
#include "FrameBufferData.h"

enum DrawingPass
{
	DrawingPassSolid,
	DrawingPassSolidForced,
	DrawingPassTransparent,
	DrawingPassDeffered,
	DrawingPassShadow
};

class MaterialData
{
private:
	ListContainer<TextureData> Textures;
	ShaderData* shader;
	ShaderData* defferedShader;
	ShaderData* shadowShader;
public:
	bool Transparent;
	char Name[MaxNameLen];
	MaterialData(void);
	~MaterialData(void);
	bool Bind(enum DrawingPass pass);
	static MaterialData* FromXml(char* source);
};

extern ListContainer<MaterialData> Materials;

