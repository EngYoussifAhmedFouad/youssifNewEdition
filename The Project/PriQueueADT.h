#pragma once
template<typename T>
class PriQueueADT
{
public:
	virtual bool isEmpty() const = 0;

	virtual bool enqueue(const T& newEntry, int newPriority) = 0;

	virtual bool dequeue(T& FrontEntry) = 0;

	virtual bool peek(T& FrontEntry) const = 0;

	virtual ~PriQueueADT() {}
};