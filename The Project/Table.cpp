#include "Table.h"
#include <iostream>
using namespace std;
#include "Order.h"

Table::Table(int id, int cap)
{
	ID = id;
	Capacity = cap;
	FreeSeats = cap;
	isSharable = true;
	Status = AVAILABLETABLE;
	Order* currOrder = nullptr;
}

int Table::Get_ID() const
{
	return ID;
}

int Table::getCapacity() const
{
	return Capacity;
}

int Table::getFreeSeats() const
{
	return FreeSeats;
}

bool Table::getisSharable() const
{
	return isSharable;
}

TBL_STATUS Table::getStatus() const { 
	return Status; 
}

bool Table::reservetable(Order* pOrd)
{
	// Check if the order exists and if the table has enough free seats
	if (pOrd && canfit(pOrd->Get_Seats())) {

		// 1. Link the order pointer
		currOrder = pOrd;

		// 2. Reduce the available seats
		FreeSeats -= pOrd->Get_Seats();

		// 3. Update sharing status
		// If the customer does NOT want to share, the table becomes non-sharable
		if (!pOrd->Get_Can_Share()) {
			isSharable = false;
		}

		// 4. Update table status
		Status = IN_SERVICETABLE;

		return true; // Success: Order assigned
	}

	return false; // Failure: Order is null or table is too small
}

void Table::releasetable(int seats)
{
	FreeSeats += seats;

	if (FreeSeats == Capacity) {
		Status = AVAILABLETABLE;
		isSharable = true;
		currOrder = nullptr;
	}
}

void Table::resettable()
{
	FreeSeats = Capacity;
}

bool Table::canfit(int seats)
{
	return FreeSeats >= seats;
}

bool Table::isFull()
{
	return FreeSeats==0;
}

bool Table::isEmpty()
{
	return FreeSeats==Capacity;
}

void Table::Print()
{
	cout << "[T" << ID << "," << Capacity << "," << FreeSeats << "]";
}

ostream& operator<< (ostream& Out,const Table& T) {
	Out << "[T" << T.Get_ID() << "," << T.getCapacity() << "," << T.getFreeSeats() << "]";
	return Out;
}