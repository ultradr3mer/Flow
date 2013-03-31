#pragma once
#include "Base.h"
#include "Model.h"
#include "Brush.h"

class MapData
{
private:
	Model* mapModel;
	//void makeFace(int index0, int index1, int index2, int index3, int textureMapping);
	void updateCollisionModel();
public:
	ListContainer<Brush> Brushes;
	ListContainer<Model> Drawables;
	MapData(void);
	~MapData(void);
	void AddBrush(Brush* newBrush);
	void GenerateMeshFromBrushes(Brush* newBrush);
	void Draw(enum DrawingPass pass);
};

