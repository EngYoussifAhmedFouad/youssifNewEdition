#pragma once
template <typename T>
class PriNode
{
private:
	T item;
	int priority;
	PriNode<T>* next;

public:
	PriNode(const T& r_Item, int pri)
	{
		item = r_Item;
		priority = pri;
		next = nullptr;
	}

	void setItem(const T& r_Item) { item = r_Item; }
	void setPriority(int pri) { priority = pri; }
	void setNext(PriNode<T>* nextNodePtr) { next = nextNodePtr; }

	T getItem() const { return item; }
	int getPriority() const { return priority; }
	PriNode<T>* getNext() const { return next; }
};