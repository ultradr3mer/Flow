#include "Filter2D.h"


Filter2D::Filter2D(void)
{
	Mesh = MeshData::FromObj("screenQuad.obj");
}


Filter2D::~Filter2D(void)
{
}

void Filter2D::Draw(MaterialData* filterMaterial)
{
	Mesh->Bind();

	if(!filterMaterial->Bind(DrawingPassSolid))
		return;

	glDrawElements(GL_TRIANGLES, Mesh->Length, GL_UNSIGNED_INT, 0);
}

void Filter2D::Draw(MaterialData* filterMaterial, ListContainer<UniformInsert>* UniformInsterts)
{
	Mesh->Bind();

	if(!filterMaterial->Bind(DrawingPassSolid))
		return;

	ShaderData::ParseUniformInserts(UniformInsterts);

	glDrawElements(GL_TRIANGLES, Mesh->Length, GL_UNSIGNED_INT, 0);
}

