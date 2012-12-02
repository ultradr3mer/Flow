#include "GameTile.h"

int GameTileCount;

GameTile::GameTile(void)
{
	Status = GameTileTypeWater;
	oldStatus = GameTileTypeWater;
	MeshData* mesh = MeshData::FromObj("plane.obj");
	ShaderData* shader = ShaderData::FromPlainText("textureSimple.vert","textureSimple.frag");
	TextureData* texture = TextureData::FromDDS("none.dds")->SetTarget(TexDiffuse);

	rotVec = 0;
	Front = new Model();
	Front->Mesh = mesh;
	Front->Shader = shader;
	Front->AppendTextureData(texture);

	Back = new Model();
	Back->Mesh = mesh;
	Back->Shader = shader;
	Back->AppendTextureData(texture);
}


GameTile::~GameTile(void)
{
}

void GameTile::Draw()
{
	Front->Draw();
	Back->Draw();
}

void GameTile::Update()
{
	if(oldStatus != Status)
		Switch();

	rotVec += -Front->Rotation->z * 0.007;
	rotVec *= 0.9f;

	Front->Rotation->z += rotVec;
	*Front->Position = vec3(Position.x*2.0f,0.0f,Position.y*2.0f);
	Front->Update();

	Back->Rotation->z = Front->Rotation->z + 0.5f;
	*Back->Position = *Front->Position;
	Back->Update();
}

void GameTile::Switch()
{
	oldStatus = Status;
	Front->Rotation->z = 0.5f;
	Back->Textures[0] = Front->Textures[0];

	switch (Status)
	{
	case GameTileTypeNone:
		Front->Textures[0] = TextureData::FromDDS("none.dds");
		break;
	case GameTileTypeWater:
		Front->Textures[0] = TextureData::FromDDS("water.dds");
		break;
	case GameTileTypeCross:
		Front->Textures[0] = TextureData::FromDDS("cross.dds");
		break;
	default:
		break;
	}
}
