#pragma once
#include"StackADT.h"
#include<iostream>
using namespace std;
template<typename T>
class ArrayStack : public StackADT<T>
{
private:
	T* items;		// Array of stack items
	int      top;                   // Index to top of stack
	const int STACK_SIZE;
	int count;

public:

	ArrayStack(int MaxSize=1000) :STACK_SIZE(MaxSize)
	{
		items = new T[STACK_SIZE];
		count = 0;
		top = -1;
	}  // end default constructor

	//Function getCapacity() returns the stack max size
	//added for array implementaion only
	int getCapacity()
	{
		return STACK_SIZE;
	}

	int GetTop() const{
		return top;
	}

	bool isEmpty() const
	{
		return top == -1;
	}  // end isEmpty

	bool push(const T& newEntry)
	{
		if (top == STACK_SIZE - 1) return false;	//Stack is FULL

		top++;
		items[top] = newEntry;
		count++;
		return true;
	}  // end push


	bool pop(T& TopEntry)
	{
		if (isEmpty()) return false;

		TopEntry = items[top];
		top--;
		count--;
		return true;
	}  // end pop

	bool peek(T& TopEntry) const
	{
		if (isEmpty()) return false;

		TopEntry = items[top];
		return true;
	}  // end peek

	int GetCount() const
	{
		return count;
	}

	void Print() const
	{
		for (int i = 0; i < count; i++)
		{
			cout << items[i];
			if (i < top)
			{
				cout << ", ";
			}
		}
	}

	//Destructor
	~ArrayStack()
	{
		delete[]items;
	}

	//Copy constructor
	ArrayStack(const ArrayStack<T>& S) :STACK_SIZE(S.STACK_SIZE)
	{
		items = new T[STACK_SIZE];
		for (int i = 0; i <= S.top; i++)
			items[i] = S.items[i];
		top = S.top;
		count = S.count;
	}


}; // end ArrayStack