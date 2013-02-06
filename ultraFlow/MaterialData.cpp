#include "MaterialData.h"

ListContainer<MaterialData> Materials;

MaterialData::MaterialData(void)
{
	//Textures = ListContainer();
	shader = nullptr;
	defferedShader = nullptr;
	shadowShader = nullptr;
	Textures.PerformCleanup = false;
}

MaterialData::~MaterialData(void)
{
}

MaterialData* MaterialData::FromXml(char* source)
{
	Materials.InitReader();
	while(Materials.Read())
	{
		if(strcmp(source,Materials.Cur->Name) == 0)
			return Materials.Cur;
	}

	MaterialData* material = new MaterialData();
	strcpy(material->Name,source);

	XmlParser* xmlParser = XmlParser::FromFile(source);
	XmlNode* rootNode = xmlParser->RootNode;

	//Read Textures
	ListContainer<XmlAttribute>* textureList = &rootNode->ChildNode("textures")->Attributes;
	textureList->InitReader();
	while (textureList->Read())
	{
		enum Uniforms uniform = GetUniformEnum(textureList->Cur->Name);
		if(uniform != UniformsNull)
			material->Textures.Add(TextureData::FromDDS(textureList->Cur->Value)->SetTarget(uniform));
	}

	//Read Frame Buffer Textures
	ListContainer<XmlAttribute>* fbBextureList = &rootNode->ChildNode("framebuffers")->Attributes;
	fbBextureList->InitReader();
	while (fbBextureList->Read())
	{
		enum Uniforms uniform = GetUniformEnum(fbBextureList->Cur->Name);
		if(uniform != UniformsNull)
			material->Textures.Add(new FbTextureBinder(fbBextureList->Cur->Name,fbBextureList->Cur->Value));
	}

	//Read Shader
	XmlNode* shaderData = rootNode->ChildNode("shader");
	if(!shaderData->IsNull())
	{
		material->shader = ShaderData::FromPlainText(
			shaderData->Attribute("frag")->Value,
			shaderData->Attribute("vert")->Value);
	}

	//Read Deffered Shader
	XmlNode* defferedShaderData = rootNode->ChildNode("defferedshader");
	if(!defferedShaderData->IsNull())
	{
		material->defferedShader = ShaderData::FromPlainText(
			defferedShaderData->Attribute("frag")->Value,
			defferedShaderData->Attribute("vert")->Value);
	}

	//Read Shadow Shader
	XmlNode* shadowShaderData = rootNode->ChildNode("shadowshader");
	if(!shadowShaderData->IsNull())
	{
		material->shadowShader = ShaderData::FromPlainText(
			shadowShaderData->Attribute("frag")->Value,
			shadowShaderData->Attribute("vert")->Value);
	}

	//Read Flags
	material->Transparent = !rootNode->ChildNode("transparent")->IsNull();

	//Cleanup
	delete xmlParser;
	Materials.Add(material);
	return material;
}

bool MaterialData::Bind(enum DrawingPass pass)
{
	if((pass == DrawingPassTransparent) != Transparent)
		return false;

	//get correct Shader
	#pragma region current Shader
	ShaderData* curShader;
	switch (pass)
	{
	case DrawingPassDeffered:
		curShader = defferedShader;
		break;
	case DrawingPassShadow:
		curShader = shadowShader;
		break;
	default:
		curShader = shader;
		break;
	}

	if(curShader == nullptr)
		return false;
	else
		curShader->Bind();
	#pragma endregion

	ShaderData::Uniform2fv(VecRenderSize,CurRenderDim);

	Textures.InitReader();
	while(Textures.Read())
	{
		Textures.Cur->Bind();
	}
	return true;
}