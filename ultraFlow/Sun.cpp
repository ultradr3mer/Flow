#include "Sun.h"

Sun* CurLight;

mat4 bias = mat4(	
	0.5f, 0.0f, 0.0f, 0.0f, 
	0.0f, 0.5f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.5f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f);

Sun::Sun(void)
{
	Mesh = MeshData::FromObj("screenQuad.obj");
	Material = MaterialData::FromXml("defferedSun.xmf");

	viewPort = new ViewPortIso();
	viewPort->Position = vec3(0);
	viewPort->Rotation = vec3(0.4f,0.04f,0.0f);
	viewPort->Update();

	ShadowBuffer.SizeX = 4096/4;
	ShadowBuffer.SizeY = 4096/4;
	ShadowBuffer.Initialize();

	shadowProjectionMatrix = bias * viewPort->ViewProjectionMatrix;
}


Sun::~Sun(void)
{
	delete viewPort;
}

void Sun::Draw()
{
	Mesh->Bind();

	if(!Material->Bind(DrawingPassSolid))
		return;

	ShaderData::Uniform3fv(VecEyePos,curViewPort->Position);
	ShaderData::Uniform3fv(VecEyeFwd,curViewPort->fwd);
	ShaderData::Uniform3fv(VecEyeRight,curViewPort->right);
	ShaderData::Uniform3fv(VecEyeUp,curViewPort->up);

	ShaderData::Uniform1f(FltNear,curViewPort->Near);
	ShaderData::Uniform1f(FltFar,curViewPort->Far);

	ShaderData::Uniform3fv(VecDirection,viewPort->fwd1);

	ShaderData::UniformMatrix4fv(MatShadowPrjection,shadowProjectionMatrix);

	glDrawElements(GL_TRIANGLES, Mesh->Length, GL_UNSIGNED_INT, 0);
}

void Sun::Update()
{
}

void Sun::Bind()
{
	ShadowBuffer.Bind();
	viewPort->Bind();
}