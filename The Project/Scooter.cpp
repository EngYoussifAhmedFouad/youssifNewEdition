#include <iostream>
using namespace std;
#include <string>
#include "Scooter.h"
#include "Order.h"

Scooter::Scooter(int id, int s, int maind, int maxorders)
{
	ID = id;
	Speed = s;
	MainDuration = maind;
	maxOrdersBeforeMain = maxorders;
	status = AVAILABLE;
	OrdersCount = 0;
	TotalDistance = 0;
	AssignedOrder = nullptr;
}

int Scooter::Get_ID() const
{
	return ID;
}

int Scooter::getOrdersCount() const
{
	return OrdersCount;
}

int Scooter::getSpeed() const
{
	return Speed;
}

int Scooter::getTotalDist() const
{
	return TotalDistance;
}

string Scooter::getStatus() const
{
	switch (status) {
	case AVAILABLE:
		return "Available";
	case IN_SERVICE:
		return "InService";
	case IN_MAINTENANCE:
		return "Maintenance";
	case BACK:
		return "Back";
	default:
		return "Available";
	}
}

Order* Scooter::getAssignedOrder() const
{
	return AssignedOrder;
}


void Scooter::AssignScooter(Order* pOrd) {
	if (status == AVAILABLE && pOrd != nullptr) {
		status = IN_SERVICE;
		AssignedOrder = pOrd;
		OrdersCount++;

		// Update criteria: track total distance for priority assignment 
		TotalDistance += pOrd->Get_Distance();
	}
}

void Scooter::FinishDel()
{
	status = BACK;
	AssignedOrder = nullptr;
}

void Scooter::Return()
{
	status = AVAILABLE;
}

void Scooter::SendToMain()
{
	status = IN_MAINTENANCE;
}

bool Scooter::NeedMain()
{
	return OrdersCount >= maxOrdersBeforeMain;
}

void Scooter::FinishMain()
{
	OrdersCount = 0;
	status = AVAILABLE;
}

void Scooter::Print()
{
	cout << "[S" << ID << ", Speed=" << Speed << ", Orders=" << OrdersCount << ", status=" << getStatus();
	if (AssignedOrder) {
		cout << ", OrderID= " << AssignedOrder->Get_ID();
	}
	cout << "]";
}

ostream& operator<<(ostream& Out,const Scooter& S)
{
	Out << "[S" << S.Get_ID() << ", Speed=" << S.getSpeed() << ", Orders=" << S.getOrdersCount() << ", status=" << S.getStatus();
	if (S.getAssignedOrder()) {
		Out << ", OrderID= " << S.getAssignedOrder()->Get_ID() << "]" << endl;
	}
		Out << "]" << endl;
	return Out;
}


