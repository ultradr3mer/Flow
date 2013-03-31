#pragma once

#pragma region Header

#pragma region ListElement
struct ListElement
{
	ListElement* NextElement;
	float SortValue;
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
	ListElement* lastElement;
	ListElement* nextElement;
	ListElement* elementToRemove;
	ListElement* currentElement;

	//Intern Movement
	void goToElement(int index);

public:
	bool PerformCleanup;
	int Length;
	T* Cur;
	T** Index;

	//Constructors
	ListContainer(void);
	~ListContainer(void);

	//Adding elements
	void Add(T* content);

	//Removes all elements and Adds the new one
	void Set(T* content);

	//Getting elements
	T* Get(int index);

	//Iterating through the list
	bool Read();
	void InitReader();

	//Removing elements
	void RemoveFirst();
	void Remove(T* content);
	void Remove(int index);
	void RemoveDelete(T* content);
	void RemoveDelete(int index);

	//Clearing the list
	void Clear();
	void ClearDelete();

	//Info
	bool Contains(T* content);
	int CalcLength();

	//Indexing
	T** GetIndex();

	// Sorting
	void SetSortValue(T* content, float value);
	void SetCurSortValue(float value);
	float GetSortValue(T* content);
	float GetCurSortValue();
	void Sort();
};
#pragma endregion

#pragma endregion

#pragma region Definitions

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
	Index = nullptr;
}

//Deconstuctor
template <class T>
inline ListContainer<T>::~ListContainer(void)
{
	if(Index != nullptr)
		delete Index;

	if(PerformCleanup)
		ClearDelete();
	else
		Clear();
}

//Adds an Elenent to the stack position will be End
template <class T>
inline void ListContainer<T>::Add(T* content)
{
	ListElement* newElement = new ListElement(content);
	if(firstElement == nullptr)
	{
		firstElement = newElement;
		lastElement = newElement;
	}
	else
	{
		lastElement->NextElement = newElement;
		lastElement = newElement;
	}

	Length++;
}

//Removes all elements and Adds the new one
template <class T>
inline void ListContainer<T>::Set(T* content)
{
	if(PerformCleanup)
		ClearDelete();
	else
		Clear();

	firstElement = new ListElement(content);
	lastElement = firstElement;

	Length = 1;
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

	Cur = (T*)nextElement->Content;
	currentElement = nextElement;
	nextElement = nextElement->NextElement;
	return true;
}

//Reset stepping to first object
template <class T>
inline void ListContainer<T>::InitReader()
{
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
	int i = 0;
	elementToRemove = nullptr;
	ListElement* curElement = firstElement;
	ListElement* prevElement = nullptr;
	InitReader();
	while (Read())
	{
		if(i == index)
		{
			if(prevElement != nullptr)
			{
				prevElement->NextElement = nextElement;
				if(nextElement == nullptr)
					lastElement = prevElement;
			}
			else
			{
				firstElement = firstElement->NextElement;
			}
				
			delete curElement;
			Length--;
			break;
		}
		prevElement = curElement;
		curElement = nextElement;
		i++;
	}
}

//Removes Element with defined content
template <class T>
inline void ListContainer<T>::Remove(T* content)
{
	elementToRemove = nullptr;
	ListElement* curElement = firstElement;
	ListElement* prevElement = nullptr;
	InitReader();
	while (Read())
	{
		if(curElement->Content == content)
		{
			if(prevElement != nullptr)
			{
				prevElement->NextElement = nextElement;
				if(nextElement == nullptr)
					lastElement = prevElement;
			}
			else
			{
				firstElement = firstElement->NextElement;
			}
				
			delete curElement;
			Length--;
		}
		prevElement = curElement;
		curElement = nextElement;
	}
}

//Removes Element with defined content and deletes this content
template <class T>
inline void ListContainer<T>::RemoveDelete(T* content)
{
	elementToRemove = nullptr;
	ListElement* curElement = firstElement;
	ListElement* prevElement = nullptr;
	InitReader();
	while (Read())
	{
		if(curElement->Content == content)
		{
			if(prevElement != nullptr)
			{
				prevElement->NextElement = nextElement;
				if(nextElement == nullptr)
					lastElement = prevElement;
			}
			else
			{
				firstElement = firstElement->NextElement;
			}
				
			delete curElement;
			Length--;

			delete content;

			//Reinitialze Reader
			elementToRemove = nullptr;
			ListElement* curElement = firstElement;
			ListElement* prevElement = nullptr;
			InitReader();
		}
		prevElement = curElement;
		curElement = nextElement;
	}
}

//Removes Element at position definded by index and deletes the Content
template <class T>
inline void ListContainer<T>::RemoveDelete(int index)
{
	int i = 0;
	elementToRemove = nullptr;
	T* contentToRemove = nullptr;
	ListElement* curElement = firstElement;
	ListElement* prevElement = nullptr;
	InitReader();
	while (Read())
	{
		if(i == index)
		{
			if(prevElement != nullptr)
			{
				prevElement->NextElement = nextElement;
				if(nextElement == nullptr)
					lastElement = prevElement;
			}
			else
			{
				firstElement = firstElement->NextElement;
			}
				
			contentToRemove = (T*)curElement->Content;
			delete curElement;
			delete contentToRemove;
			Length--;
			break;
		}
		prevElement = curElement;
		curElement = nextElement;
		i++;
	}
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
	if(Index != nullptr)
		delete Index;

	Index = new T*[Length];
	int i = 0;

	InitReader();
	while (Read())
	{
		Index[i++] = Cur;
	}

	return Index;
}

//Deletes all elements
template <typename T>
inline void ListContainer<T>::ClearDelete(void)
{
	while (Length > 0)
	{
		RemoveDelete(0);

		//T** index = GetIndex();
		//printf("\n%d",CalcLength());
	}
}

//Deletes all elements
template <typename T>
inline int ListContainer<T>::CalcLength(void)
{
	int count = 0;

	InitReader();
	while (Read())
	{
		count ++;
	}

	return count;
}

//Set sorting value
template <typename T>
inline void ListContainer<T>::SetSortValue(T* content, float value)
{
	ListElement* curElement = firstElement;
	while (curElement != nullptr)
	{
		if(curElement->Content == content)
		{
			curElement->SortValue = value;
			break;
		}
		curElement = curElement->NextElement;
	}
}

//Set sorting value
template <typename T>
inline void ListContainer<T>::SetCurSortValue(float value)
{
	currentElement->SortValue = value;
}

//Set sorting value
template <typename T>
inline float ListContainer<T>::GetCurSortValue()
{
	return currentElement->SortValue;
}

//Get sorting value
template <typename T>
inline float ListContainer<T>::GetSortValue(T* content)
{
	ListElement* curElement = firstElement;
	while (curElement != nullptr)
	{
		if(curElement->Content == content)
		{
			return curElement->SortValue;
		}
		curElement = curElement->NextElement;
	}
	return -1.0f;
}

//Sort all elements
template <typename T>
inline void ListContainer<T>::Sort(void)
{
	//Variables
	int segmentPosition = 0;
	ListElement* leftFirstElement;
	ListElement* rightFirstElement;
	ListElement* tmpElement;
	//ListElement* curFirstElement = firstElement;
	ListElement* firstProcessedElement;
	ListElement* lastProcessedElement;

	//Actions
	for(int curentSegment = 1; curentSegment <= Length; curentSegment *= 2)
	{
		firstProcessedElement = nullptr;
		while(firstElement != nullptr)
		{
			//Generate left side
			leftFirstElement = firstElement;

			if(firstElement != nullptr)
			{
				tmpElement = firstElement; 
				for (int i = 0; i < curentSegment - 1 && tmpElement->NextElement != nullptr; i++)
				{
					tmpElement = tmpElement->NextElement;
				}
				firstElement = tmpElement->NextElement;
				tmpElement->NextElement = nullptr;
			}

			//Generate right side
			rightFirstElement = firstElement;

			if(firstElement != nullptr)
			{
				tmpElement = firstElement; 
				for (int i = 0; i < curentSegment - 1 && tmpElement->NextElement != nullptr; i++)
				{
					tmpElement = tmpElement->NextElement;
				}
				firstElement = tmpElement->NextElement;
				tmpElement->NextElement = nullptr;
			}

			//Merge sides together
			if(leftFirstElement != nullptr && rightFirstElement != nullptr)
			{
				//Initialize list
				if(firstProcessedElement == nullptr)
				{
					if(leftFirstElement->SortValue < rightFirstElement->SortValue)
					{
						firstProcessedElement = leftFirstElement;
						lastProcessedElement = leftFirstElement;
						leftFirstElement = leftFirstElement->NextElement;
					}
					else
					{
						firstProcessedElement = rightFirstElement;
						lastProcessedElement = rightFirstElement;
						rightFirstElement = rightFirstElement->NextElement;
					}
				}

				//Add item depending on value
				while(leftFirstElement != nullptr && rightFirstElement != nullptr)
				{
					if(leftFirstElement->SortValue < rightFirstElement->SortValue)
					{
						lastProcessedElement->NextElement = leftFirstElement;
						lastProcessedElement = leftFirstElement;
						leftFirstElement = leftFirstElement->NextElement;
					}
					else
					{
						lastProcessedElement->NextElement = rightFirstElement;
						lastProcessedElement = rightFirstElement;
						rightFirstElement = rightFirstElement->NextElement;
					}
				}
			}
			else if (firstProcessedElement == nullptr)
			{
				//One list is empty -> no initialisation
				if(leftFirstElement != nullptr)
				{
					firstProcessedElement = leftFirstElement;
					lastProcessedElement = leftFirstElement;
					leftFirstElement = leftFirstElement->NextElement;
				}
				else
				{
					firstProcessedElement = rightFirstElement;
					lastProcessedElement = rightFirstElement;
					rightFirstElement = rightFirstElement->NextElement;
				}
			}

			//Fill remaining Parts
			while(leftFirstElement != nullptr)
			{
				lastProcessedElement->NextElement = leftFirstElement;
				lastProcessedElement = leftFirstElement;
				leftFirstElement = leftFirstElement->NextElement;
			}
			while(rightFirstElement != nullptr)
			{
				lastProcessedElement->NextElement = rightFirstElement;
				lastProcessedElement = rightFirstElement;
				rightFirstElement = rightFirstElement->NextElement;
			}
		}
		firstElement = firstProcessedElement;
	}
}

//Removes first element from list
template <typename T>
inline void ListContainer<T>::RemoveFirst(void)
{
	firstElement = firstElement->NextElement;
	if(firstElement == nullptr)
		lastElement = nullptr;

	Length--;
}

//Finds out if the list contains this item
template <class T>
inline bool ListContainer<T>::Contains(T* content)
{
	nextElement = firstElement;
	while (nextElement != nullptr)
	{
		if(nextElement->Content == content)
			return true;

		nextElement = nextElement->NextElement;
	}

	return false;
}
#pragma endregion

#pragma endregion