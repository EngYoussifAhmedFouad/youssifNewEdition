#pragma once
#include "QueueADT.h"
#include"Node.h"
#include <iostream>
using namespace std;
template<typename T>
class LinkedQueue : public QueueADT<T>
{
protected:

	Node<T>* backPtr;
	Node<T>* frontPtr;
	int count;
public:
	LinkedQueue()
	{
		backPtr = nullptr;
		frontPtr = nullptr;
		count = 0;

	}
	bool isEmpty() const
	{
		return (frontPtr == nullptr);
	}

	bool enqueue(const T& newEntry)
	{
		Node<T>* newNodePtr = new Node<T>(newEntry);
		// Insert the new node
		if (isEmpty())	//special case if this is the first node to insert
			frontPtr = newNodePtr; // The queue is empty
		else
			backPtr->setNext(newNodePtr); // The queue was not empty

		backPtr = newNodePtr; // New node is the last node now
		count++;
		return true;
	} // end enqueue

	bool dequeue(T& frntEntry)
	{
		if (isEmpty())
			return false;

		Node<T>* nodeToDeletePtr = frontPtr;
		frntEntry = frontPtr->getItem();
		frontPtr = frontPtr->getNext();
		// Queue is not empty; remove front
		if (nodeToDeletePtr == backPtr)	 // Special case: last node in the queue
			backPtr = nullptr;

		// Free memory reserved for the dequeued node
		delete nodeToDeletePtr;
		count--;
		return true;

	}
	bool peek(T& frntEntry)  const
	{
		if (isEmpty())
			return false;

		frntEntry = frontPtr->getItem();
		return true;

	}
	~LinkedQueue()
	{
		T temp;

		//Free (Dequeue) all nodes in the queue
		while (dequeue(temp));
	}

	//copy constructor
	LinkedQueue(const LinkedQueue<T>& LQ)
	{
		Node<T>* NodePtr = LQ.frontPtr;
		if (!NodePtr) //LQ is empty
		{
			frontPtr = backPtr = nullptr;
			return;
		}

		//insert the first node
		Node<T>* ptr = new Node<T>(NodePtr->getItem());
		frontPtr = backPtr = ptr;
		NodePtr = NodePtr->getNext();

		//insert remaining nodes
		while (NodePtr)
		{
			Node<T>* ptr = new Node<T>(NodePtr->getItem());
			backPtr->setNext(ptr);
			backPtr = ptr;
			NodePtr = NodePtr->getNext();
		}
		count = LQ.count;
	}
	int GetCount()
	{
		return count;
	}
	void Print()
	{
		Node<T>* ptr = frontPtr;
		int i = 0;
		while (ptr != nullptr && i < count)
		{
			cout << ptr->getItem();
			i++;
			ptr = ptr->getNext();
			if (i < count)
				cout << ", ";
		}
	}

	Node<T>* getFront() const
	{
		return frontPtr;
	}
};