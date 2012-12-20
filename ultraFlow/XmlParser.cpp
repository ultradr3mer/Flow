#include "XmlParser.h"

XmlAttribute nullAttribute = XmlAttribute();
XmlNode nullNode = XmlNode();

#pragma region XmlObjects

#pragma region XmlAttribute
XmlAttribute::XmlAttribute(char* Content)
{
	int pos = 0;
	cutFromChar(Name,'=',Content,&pos);
	char tmpValue[valueLength];
	cutFromChar(tmpValue,'\0',Content,&pos);

	if(tmpValue[0] = '\"')
	{
		pos = 1;
		cutFromChar(Value,'\"',tmpValue,&pos);
		IsNumeric = false;
	}
	else
	{
		strcpy(Value,tmpValue);
		IsNumeric = false;
	}
}
XmlAttribute::XmlAttribute()
{
	strcpy(Name,"NULL");
	strcpy(Value,"NULL");
}
#pragma endregion

#pragma region XmlNode
char nodeHeadBuffer[contentLength];
XmlNode::XmlNode()
{
	strcpy(Name,"NULL");
	//ChildNodes = ListContainer();
	//Attributes = ListContainer();
	ParentNode = nullptr;
}
XmlNode::XmlNode(char* Content, XmlNode* Parent)
{
	ParentNode = Parent;

	int pos = 0;
	int posUnused = 0;
	if(Content[0] == '/')
	{
		pos = 1;
		NodeType = XmlNodeTypeEnd;
	}
	else if(Content[strlen(Content)-1] == '/')
	{
		Content[strlen(Content)-1] = '\0';
		NodeType = XmlNodeTypeBoth;
	}
	else
	{
		NodeType = XmlNodeTypeStart;
	}

	//ChildNodes = ListContainer();
	//Attributes = ListContainer();
	cutFromChar(Name,' ',Content,&pos);
	while (cutFromChar(nodeHeadBuffer,' ',Content,&pos))
	{
		Attributes.Add(new XmlAttribute(nodeHeadBuffer));
	}
}
XmlNode::~XmlNode()
{
}
bool XmlNode::IsNull()
{
	return strcmp("NULL",Name) == 0;
}
XmlNode* XmlNode::ChildNode(char* Name)
{
	XmlNode* xmlNode = nullptr;
	ChildNodes.InitReader(&xmlNode);
	while(ChildNodes.Read())
	{
		if(strcmp(xmlNode->Name,Name) == 0)
			return xmlNode;
	}
	return &nullNode;
}
XmlAttribute* XmlNode::Attribute(char* Name)
{
	XmlAttribute* xmlAttribute = nullptr;
	Attributes.InitReader(&xmlAttribute);
	while(Attributes.Read())
	{
		if(strcmp(xmlAttribute->Name,Name))
			return xmlAttribute;
	}
	return &nullAttribute;
}
#pragma endregion

#pragma endregion

#pragma region XmlParser
char contentBuffer[contentLength];
char headBuffer[contentLength];
XmlParser::XmlParser(char* Text)
{
	bool isstring = false;
	int pos = 0;
	XmlNode* curParent = nullptr;
	XmlNode* curNode = nullptr;
	while (cutFromChar(contentBuffer,'<',Text,&pos))
	{
		cutFromChar(contentBuffer,'>',Text,&pos);
		curNode = new XmlNode(contentBuffer,curParent);

		if(curParent == nullptr)
		{
			curParent = curNode;
			RootNode = curNode;
		}
		else
		{
			curParent->ChildNodes.Add(curNode);
		}

		switch (curNode->NodeType)
		{
			case XmlNodeTypeStart:
				curParent = curNode;
				break;
			case XmlNodeTypeEnd:
				curParent = curParent->ParentNode;
				break;
			default:
				break;
		}
	}
}
XmlParser::~XmlParser(void)
{
	delete RootNode;
}
XmlParser* XmlParser::FromFile(char* file)
{
	char* data = FileToBuf(FullFileName("materials\\",file));
	return new XmlParser(data);
}
#pragma endregion