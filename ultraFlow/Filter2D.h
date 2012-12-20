#pragma once
#include "model.h"
class Filter2D :
	public Model
{
public:
	Filter2D(void);
	~Filter2D(void);
	void Draw(MaterialData* filterMaterial);
};

