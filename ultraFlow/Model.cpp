#include "Model.h"
#include "ViewPort.h"

//Model::Model(Scene* scene)
//{
//	//Textures = nullptr;
//	Mesh = nullptr;
//	Material = nullptr;
//	this->scene = scene;
//	scene->SceneDrawables.Add(this);
//}

Model::Model()
{
	//Textures = nullptr;
	Mesh = nullptr;
	Material = nullptr;
	//Size = vec3(1.0f);
}


Model::~Model(void)
{
	//scene->SceneDrawables.Remove(this);
}

void Model::Draw()
{
	Draw(DrawingPassSolid);
}

void Model::Draw(enum DrawingPass pass)
{
	if(pass == DrawingPassSolidForced || curViewPort->FrustumCheck(Position, Mesh->maxRadius))
	{
		Mesh->Bind();

		if(!Material->Bind(pass))
			return;

		setupMatrices();
		ShaderData::ParseUniformInserts(&UniformInserts);

		glDrawElements(GL_TRIANGLES, Mesh->Length, GL_UNSIGNED_INT, 0);
	}
}

void Model::setupMatrices()
{
	ShaderData::UniformMatrix4fv(MatModelView, modelMatrix);
	ShaderData::UniformMatrix4fv(MatViewProjection, curViewPort->ViewProjectionMatrix);
}

void Model::Update()
{
	if(Position != oldPosition || Rotation  != oldRotation)
	{
		modelMatrix = MatrixFromPosAng(Position,Rotation);
		oldPosition = Position;
		oldRotation = Rotation;
	}
}