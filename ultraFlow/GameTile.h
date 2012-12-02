#include "Base.h"
#include "Model.h"
#pragma once
extern int GameTileCount;

enum GameTileType
{
	GameTileTypeNone,
	GameTileTypeWater,
	GameTileTypeCross
};

class GameTile
{
private:
	Model* Front;
	Model* Back;
	float rotVec;
	enum GameTileType oldStatus;
	void Switch();

public:
	GameTile(void);
	~GameTile(void);
	void Update();
	void Draw();
	vec2 Position;
	enum GameTileType Status;
};

