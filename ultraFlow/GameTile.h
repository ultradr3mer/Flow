#include "Base.h"
#include "Model.h"
#pragma once
extern int GameTileCount;

enum GameTileType
{
	Water,
	Ship
};

class GameTile
{
private:
	Model* Front;
	Model* Back;
public:
	GameTile(void);
	~GameTile(void);
	void Update();
	void Draw();
	vec2 Position;
};

