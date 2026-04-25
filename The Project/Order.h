#pragma once
#include "GenericDefs.h"
#include <iostream>
using namespace std;

// Forward declarations to avoid circular includes
class Chefs;
class Table;
class Scooter;
class Order
{
private:
	int ID; //Each Order has ID
	int TQ;  //Request timestep
	int TA; // Assignment time
	int TR; // Ready time
	int TS; // Service start time
	int TF; // Finish time
	int Size; //Number of Dishes
	int Price; //The Price
	ORD_TYPE Type; //The type of Order

	//For Dine_in Orders "OD"
	int Seats;  //Number of Seats
	int Duration;  // the period a table should be reserved for this order
	bool Can_Share;  //Sharable or not?

	//For Delivery Orders Only "OV"
	float Distance;  //In meters



	//for Cooking Orders
	int AssignedChefID;
	Chefs* pAssignedChef;
	Table* pAssignedTable;
	Scooter* pAssignedScooter;


public:
	//Order();
	Order(int id, ORD_TYPE type, int size, int price, int tq);
	//The getters

	int Get_ID() const;
	int Get_TQ() const;
	int Get_Size() const;
	int Get_Price() const;
	ORD_TYPE Get_Type() const;
	//For "OD"
	int Get_Seats() const;
	int Get_Duration() const;
	bool Get_Can_Share() const;
	//For "OV"
	float Get_Distance() const;

	//The Setters
	//To Set The times
	void SetTA(int time);
	void SetTR(int time);
	void SetTS(int time);
	void SetTF(int time);
	//For "OD"
	void Set_Seats(int S);
	void Set_Duration(int D);
	void Set_Can_Share(bool CS);
	//For "OV"
	void Set_Distance(float D);

	//The time Metrics
	//Ideal time
	int Get_Ti() const;
	//cook period
	int Get_Tc() const;
	//Wait time
	int Get_Tw() const;
	//Sevice duration
	int Get_Tserv() const;

	//For The cooking Orders
	void SetAssignedChefID(int id);
	int GetAssignedChefID() const;

	// << operator
	friend ostream& operator<<(ostream& C, const Order* pOrd);
	void setAssignedChef(Chefs* pCh);
	Chefs* getAssignedChef();
	void setAssignedTable(Table* pT);
	Table* getAssignedTable();
	void setAssignedScooter(Scooter* pS);
	Scooter* getAssignedScooter();

};