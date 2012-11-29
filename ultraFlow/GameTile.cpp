#include "GameTile.h"

int GameTileCount;

GameTile::GameTile(void)
{
	Front = new Model();
	Front->Mesh = MeshData::FromObj("plane.obj");
	Front->Shader = ShaderData::FromPlainText("textureSimple.vert","textureSimple.frag");
	Front->AppendTextureData(TextureData::FromDDS("none.dds")->SetTarget(TexDiffuse));
}


GameTile::~GameTile(void)
{
}

void GameTile::Draw()
{
	Front->Draw();
}

void GameTile::Update()
{
	*Front->Position = vec3(Position.x*2.0f,0.0f,Position.y*2.0f);
	Front->Update();
}
