//#include "ListContainer.h"
#include "GenFunc.h"

#pragma once

const int nameLength = 32;
const int valueLength = 128;
const int contentLength = 2048;

#pragma region XmlObjects
struct XmlAttribute
{
	char Name[nameLength];
	char Value[valueLength];
	bool IsNumeric;

	XmlAttribute(char* Content);
	XmlAttribute();
};
enum XmlNodeType
{
	XmlNodeTypeStart,
	XmlNodeTypeEnd,
	XmlNodeTypeBoth
};
struct XmlNode
{
	char Name[nameLength];
	ListContainer<XmlNode> ChildNodes;
	ListContainer<XmlAttribute> Attributes;
	enum XmlNodeType NodeType;
	XmlNode* ParentNode;

	XmlNode* ChildNode(char* Name);
	XmlAttribute* Attribute(char* Name);

	XmlNode();
	XmlNode(char* Content, XmlNode* Parent);
	bool IsNull();
	~XmlNode();
};
#pragma endregion

#pragma region XmlParser
class XmlParser
{
public:
	XmlNode* RootNode;
	XmlParser(char* Text);
	~XmlParser(void);
	static XmlParser* FromFile(char* file);
};
#pragma endregion
