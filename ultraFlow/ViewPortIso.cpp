#include "ViewPortIso.h"


ViewPortIso::ViewPortIso(void)
{
	float size = 30;
	size /= 2;
	ProjectrionMatrix = ortho(-size, size, size, -size, 100.0f, -100.0f);
}


ViewPortIso::~ViewPortIso(void)
{
}
