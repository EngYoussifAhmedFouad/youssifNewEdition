#pragma once
#include <iostream>
#include"PriNode.h"
#include"PriQueueADT.h"
using namespace std;

template <typename T>
class LinkedPriorityQueue :public PriQueueADT<T>
{
protected:
	PriNode<T>* frontPtr;
	PriNode<T>* backPtr;
	int count;

public:
	LinkedPriorityQueue()
	{
		frontPtr = nullptr;
		count = 0;
	}

	bool isEmpty() const
	{
		return frontPtr == nullptr;
	}

	bool enqueue(const T& newEntry, int newPriority)
	{
		PriNode<T>* newNodePtr = new PriNode<T>(newEntry, newPriority);

		if (isEmpty() || newPriority > frontPtr->getPriority())
		{
			newNodePtr->setNext(frontPtr);
			frontPtr = newNodePtr;
		}
		else
		{
			PriNode<T>* ptr = frontPtr;

			while (ptr->getNext() != nullptr && ptr->getNext()->getPriority() >= newPriority)
			{
				ptr = ptr->getNext();
			}

			newNodePtr->setNext(ptr->getNext());
			ptr->setNext(newNodePtr);
		}

		count++;
		return true;
	}

	bool dequeue(T& frntEntry)
	{
		if (isEmpty()) return false;

		PriNode<T>* nodeToDeletePtr = frontPtr;
		frntEntry = frontPtr->getItem();
		frontPtr = frontPtr->getNext();

		if (nodeToDeletePtr == backPtr)	 // Special case: last node in the queue
			backPtr = nullptr;

		delete nodeToDeletePtr;
		count--;
		return true;
	}

	bool peek(T& frntEntry) const
	{
		if (isEmpty()) return false;

		frntEntry = frontPtr->getItem();
		return true;
	}

	int GetCount() const
	{
		return count;
	}

	void print() const
	{
		PriNode<T>* ptr = frontPtr;
		while (ptr != nullptr)
		{
			//cout << ptr->getItem();

			cout << ptr->getItem();

			if (ptr->getNext() != nullptr)
				cout << ", ";

			ptr = ptr->getNext();
		}
	}

	PriNode<T>* getFront() const
	{
		return frontPtr;
	}

	~LinkedPriorityQueue()
	{
		T temp;
		while (dequeue(temp));
	}
};