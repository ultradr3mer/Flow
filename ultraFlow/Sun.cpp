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

	float shadowSize = 60;
	float innerRelativeSize = 0.3f;

	viewPort = new ViewPort();
	shadowSize /= 2;
	viewPort->ProjectrionMatrix = ortho(-shadowSize, shadowSize, shadowSize, -shadowSize, 100.0f, -100.0f);
	viewPort->Position = vec3(0);
	viewPort->Rotation = vec3(0.4f,0.04f,0.0f);
	viewPort->Update();

	viewInnerPort = new ViewPort();
	shadowSize *= innerRelativeSize;
	viewInnerPort->ProjectrionMatrix = ortho(-shadowSize, shadowSize, shadowSize, -shadowSize, 100.0f, -100.0f);
	viewInnerPort->Position = vec3(0);
	viewInnerPort->Rotation = vec3(0.4f,0.04f,0.0f);
	viewInnerPort->Update();


	ShadowBuffer.SizeX = 1024;
	ShadowBuffer.SizeY = 1024;
	ShadowBuffer.UseColor = false;
	ShadowBuffer.Initialize();

	ShadowInnerBuffer.SizeX = 1024;
	ShadowInnerBuffer.SizeY = 1024;
	ShadowInnerBuffer.UseColor = false;
	ShadowInnerBuffer.Initialize();

	shadowProjectionMatrix = bias * viewPort->ViewProjectionMatrix;
}


Sun::~Sun(void)
{
	delete viewPort;
	delete viewInnerPort;
}

void Sun::Draw()
{
	Mesh->Bind();

	if(!Material->Bind(DrawingPassSolid))
		return;

	ShaderData::Uniform3fv(VecEyePos,curViewPort->Position);
	ShaderData::Uniform3fv(VecEyeFwd,curViewPort->fwdD);
	ShaderData::Uniform3fv(VecEyeRight,curViewPort->rightD);
	ShaderData::Uniform3fv(VecEyeUp,curViewPort->upD);

	ShaderData::Uniform1f(FltNear,curViewPort->Near);
	ShaderData::Uniform1f(FltFar,curViewPort->Far);

	ShaderData::Uniform3fv(VecDirection,viewPort->fwd);

	ShaderData::UniformMatrix4fv(MatShadowProjection,shadowProjectionMatrix);
	ShaderData::UniformMatrix4fv(MatInnerShadowProjection,shadowInnerProjectionMatrix);

	glDrawElements(GL_TRIANGLES, Mesh->Length, GL_UNSIGNED_INT, 0);
}

void Sun::Update()
{
	if(length(Position - viewPort->Position)>0.5f)
	{
		viewPort->Position = Position;
		viewPort->Update();
		shadowProjectionMatrix = bias * viewPort->ViewProjectionMatrix;

		viewInnerPort->Position = Position;
		viewInnerPort->Update();
		shadowInnerProjectionMatrix = bias * viewInnerPort->ViewProjectionMatrix;
	}
}

void Sun::UpdateShadowBuffer(drawShadowCall draw)
{
	ShadowBuffer.Bind();
	viewPort->Bind();
	draw();

	ShadowInnerBuffer.Bind();
	viewInnerPort->Bind();
	draw();
}