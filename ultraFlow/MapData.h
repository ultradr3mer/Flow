#pragma once
#include "Base.h"
#include "Model.h"
#include "MapMeshData.h"

struct Brush
{
	vec3 Position;
	vec3 Size;
	bool Substract;
	uint Texture;

	inline Brush(vec3 position, vec3 size, uint texture, bool substract)
	{
		Position = position;
		Size = size;
		Texture = texture;
		Substract = substract;
	};
};

class MapData
{
private:
	Model* mapModel;
	void makeFace(int index0, int index1, int index2, int index3, int textureMapping);
	void updateCollisionModel();
public:
	ListContainer<Brush> Brushes;
	ListContainer<Model> Drawables;
	MapData(void);
	~MapData(void);
	void AddBrush(Brush* newBrush);
	void GenerateMeshFromBrushes();
	void Draw(enum DrawingPass pass);
};

