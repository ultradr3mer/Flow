#define LISTCONTAINER_CPP_
#ifndef LISTCONTAINER_H_
    #define LISTCONTAINER_H_
#include"ListContainer.h"
#endif

#pragma region ListElement
inline ListElement::ListElement(void* content)
{
	Content = content;
	NextElement = nullptr;
}
#pragma endregion

#pragma region ListContainer
//Constructor
template <class T>
inline ListContainer<T>::ListContainer(void)
{
	PerformCleanup = true;
	firstElement = nullptr;
	nextElement = nullptr;
	Length = 0;
}

//Deconstuctor
template <class T>
inline ListContainer<T>::~ListContainer(void)
{
	if(PerformCleanup)
		ClearDelete();
	else
		Clear();
}

//Adds an Elenent to the stack position will be End
template <class T>
inline void ListContainer<T>::Add(T* content)
{
	if(Length == 0)
	{
		firstElement = new ListElement(content);
	}
	else
	{
		goToElement(Length-1);
		nextElement->NextElement = new ListElement(content);
	}
	Length++;
}

//Sets nextElement to element at index
template <class T>
inline void ListContainer<T>::goToElement(int index)
{
	if(index >= Length)
	{
		nextElement = nullptr;
		return;
	}

	nextElement = firstElement;
	while (index > 0)
	{
		nextElement = nextElement->NextElement;
		index--;
	}
}

//Stepp through list
template <class T>
inline bool ListContainer<T>::Read()
{
	if(nextElement == nullptr)
		return false;

	*outPointer = (T*)nextElement->Content;
	nextElement = nextElement->NextElement;
	return true;
}

//Reset stepping to first object
template <class T>
inline void ListContainer<T>::InitReader(T** outPointer)
{
	this->outPointer = outPointer;
	nextElement = firstElement;
}

//Gets Content at position definded by index
template <class T>
inline T* ListContainer<T>::Get(int index)
{
	goToElement(index);

	if(nextElement != nullptr)
		return (T*)nextElement->Content;
	else
		return nullptr;
}

//Removes Element at position definded by index
template <class T>
inline void ListContainer<T>::Remove(int index)
{
	if(Length == 0 || index >= Length)
		return;

	if(index == 0)
	{
		ListElement* tmp = firstElement;
		firstElement = firstElement->NextElement;
		//delete (T*)tmp->Content;
		delete tmp;
	}
	else
	{
		goToElement(index-1);

		elementToRemove = nextElement->NextElement;
		nextElement->NextElement = nextElement->NextElement->NextElement;

		//delete (T*)elementToRemove->Content;
		delete elementToRemove;
	}

	Length--;
}

//Removes Element with defined content
template <class T>
inline void ListContainer<T>::Remove(T* content)
{
	T* unused = nullptr;

	elementToRemove = nullptr;
	ListElement* curElement = firstElement;
	ListElement* prevElement = nullptr;
	InitReader(&unused);
	while (Read())
	{
		if(curElement->Content == content)
		{
			if(prevElement != nullptr)
				curElement->NextElement = nextElement;
			else
				firstElement = nextElement;
				
			delete curElement;
		}
		prevElement = curElement;
		curElement = nextElement;
	}

	Length--;
}

//Removes Element with defined content and deletes this content
template <class T>
inline void ListContainer<T>::RemoveDelete(T* content)
{
	T* unused = nullptr;

	elementToRemove = nullptr;
	ListElement* curElement = firstElement;
	InitReader(&unused);
	while (Read())
	{
		if(curElement->Content == content)
		{
			prevElement->NextElement = nextElement->NextElement;
			delete (T*)content;
			delete nextElement;
			Length--;
			break;
		}
		curElement = nextElement;
	}
}

//Removes Element at position definded by index and deletes the Content
template <class T>
inline void ListContainer<T>::RemoveDelete(int index)
{
	if(Length == 0 || index >= Length)
		return;

	T* content;

	if(index == 0)
	{
		ListElement* tmp = firstElement;
		firstElement = firstElement->NextElement;
		content = (T*)tmp->Content;
		delete content;
		delete tmp;
	}
	else
	{
		goToElement(index-1);

		elementToRemove = nextElement->NextElement;
		nextElement->NextElement = nextElement->NextElement->NextElement;

		content = (T*)elementToRemove->Content;
		delete content;
		delete elementToRemove;
	}

	Length--;
}

//Removes all Elements from the list
template <class T>
inline void ListContainer<T>::Clear()
{
	T* objToRemove = nullptr;
	while (Length > 0)
	{
		Remove(0);
	}
}

//Gets index of all elements
template <class T>
inline T** ListContainer<T>::GetIndex()
{
	T** IndexBuffer = nullptr;
	if(IndexBuffer != nullptr)
		delete IndexBuffer;

	IndexBuffer = new T*[Length];
	int i = 0;

	T* content = nullptr;
	InitReader(&content);
	while (Read())
	{
		IndexBuffer[i++] = content;
	}

	return IndexBuffer;
}

//Deletes all elements
template <typename T>
inline void ListContainer<T>::ClearDelete(void)
{
	T* objToRemove = nullptr;
	while (Length > 0)
	{
		RemoveDelete(0);
	}
}
