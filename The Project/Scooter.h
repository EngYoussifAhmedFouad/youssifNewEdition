#pragma once
#include <iostream>
using namespace std;
#include <string>
#include "GenericDefs.h"
// Forward declaration to avoid circular include
class Order;
class Scooter
{
private:

	int ID;
	int Speed;
	int MainDuration;
	int OrdersCount;
	int maxOrdersBeforeMain;
	int TotalDistance;
	Status status;
	Order* AssignedOrder;



public:
	Scooter(int id,int s,int maind,int maxorders);

	int Get_ID() const;
	int getOrdersCount() const;
	int getSpeed() const;
	int getTotalDist() const;
	string getStatus() const;
	Order* getAssignedOrder() const;


	void AssignScooter(Order* pOrd);
	void FinishDel();
	void Return();


	void SendToMain();
	bool NeedMain();
	void FinishMain();

	void Print();
	friend ostream& operator<<(ostream& Out,const Scooter& S);




	Scooter() = default;
};

