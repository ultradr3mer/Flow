#pragma once

#pragma region ListElement
struct ListElement
{
	ListElement* NextElement;
	void* Content;
	ListElement(void* content);
};
#pragma endregion

#pragma region ListContainer
template <class T>
class ListContainer
{
private:
	ListElement* firstElement;
	//ListElement* lastElement;
	ListElement* nextElement;
	ListElement* elementToRemove;
	void goToElement(int index);
	T** outPointer;
public:
	bool PerformCleanup;
	int Length;

	//Adding elements
	void Add(T* content);

	//Getting elements
	T* Get(int index);

	//Reading the list
	bool Read();
	void InitReader(T** outObject);
	//Removing elements
	void Remove(T* content);
	void Remove(int index);
	void RemoveDelete(T* content);
	void RemoveDelete(int index);
	//Clearing the list
	void Clear();
	void ClearDelete();
	T** GetIndex();
	//void Deconstruct();
	//void SetIndex(void **);
	ListContainer(void);
	~ListContainer(void);
};
#pragma endregion


#define LISTCONTAINER_H_
#ifndef LISTCONTAINER_CPP_
    #define LISTCONTAINER_CPP_
#include "ListContainer.cpp"
#endif

