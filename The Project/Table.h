#pragma once
#include <iostream>
#include "GenericDefs.h"
#include "LinkedQueue.h"
// Forward declaration to avoid circular include
class Order;
using namespace std;
class Table
{
private:
	int ID;
	int Capacity;
	int FreeSeats;
	bool isSharable;
	TBL_STATUS Status;
	LinkedQueue<Order*> currOrders;
public:
	Table(int id, int cap);

	int Get_ID() const;
	int getCapacity() const;
	int getFreeSeats() const;
	bool getisSharable() const;
	TBL_STATUS getStatus() const;

	bool AssignTable(Order* pOrd);
	void releasetable(int seats);
	void resettable();

	bool canfit(int seats);
	bool isFull();
	bool isEmpty();

	void Print();
	friend ostream& operator<< (ostream& Out, const Table& T);


	Table() = default;
};