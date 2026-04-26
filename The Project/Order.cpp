#include "Order.h"
#include <iostream>
using namespace std;
//Order::Order()
//{
//    ID = 0;
//    Type = ODN; //By defalte 'for Examble'
//    Size = 0;
//    Price = 0;
//    TQ = 0;
//    TA = -1; 
//    TR = -1;
//    TS = -1;
//    TF = -1;
//
//
//    Seats = 0;
//    Duration = 0;
//    Can_Share = false;
//    Distance = 0;
//}

Order::Order(int id, ORD_TYPE type, int size, int price, int tq) :AssignedChefID(-1)
{
	ID = id;
	Type = type;
	Size = size;
	Price = price;
	TQ = tq;

	//the remain times are not declare in the begining
	TA = -1;
	TR = -1;
	TS = -1;
	TF = -1;

	Seats = 0;
	Duration = 0;
	Can_Share = false;
	Distance = 0;
}

int Order::Get_ID() const
{
	return ID;
}

int Order::Get_TQ() const
{
	return TQ;
}

int Order::Get_Size() const
{
	return Size;
}

int Order::Get_Price() const
{
	return Price;
}

ORD_TYPE Order::Get_Type() const
{
	return Type;
}

int Order::Get_Seats() const
{
	return Seats;
}

int Order::Get_Duration() const
{
	return Duration;
}

bool Order::Get_Can_Share() const
{
	return Can_Share;
}

float Order::Get_Distance() const
{
	return Distance;
}

void Order::SetTA(int time)
{
	TA = time;
}

void Order::SetTR(int time)
{
	TR = time;
}

void Order::SetTS(int time)
{
	TS = time;
}

void Order::SetTF(int time)
{
	TF = time;
}

void Order::SetTCancel(int time)
{
	TCancel = time;
}

void Order::Set_Seats(int S)
{
	Seats = S;
}

void Order::Set_Duration(int D)
{
	Duration = D;
}

void Order::Set_Can_Share(bool CS)
{
	Can_Share = CS;
}

void Order::Set_Distance(float D)
{
	Distance = D;
}

int Order::Get_Ti() const //Ti = (TA - TQ) + (TS - TR) 
{
	int Ti = 0;
	if (TA != -1) {
		Ti = TA - TQ;
	}
	if (TS != -1 && TR != -1) {
		Ti += TS - TR;
	}
	return Ti;
}

int Order::Get_Tc() const //Tc = TR - TA 
{
	if (TR != -1 && TA != -1) return TR - TA;
	return 0;
}

int Order::Get_Tw() const // Tw = Ti + Tc 
{
	int Tw = Get_Ti() + Get_Tc();
	return Tw;
}

int Order::Get_Tserv() const //Tserv = TF - TS
{
	int Tserv = 0;
	if (TF != -1 && TS != -1) {
		Tserv = TF - TS;
	}
	return Tserv;
}

int Order::Get_TCancel() const
{
	return TCancel;
}

int Order::Get_TR() const
{
	return TR;
}

int Order::Get_Tf() const
{
	return TF;
}



ostream& operator<<(ostream& C, const Order* pOrd)
{
	if (pOrd != nullptr) {
		string typeStr = "";
		switch (pOrd->Type) {
		case ODG: typeStr = "ODG"; break;
		case ODN: typeStr = "ODN"; break;
		case OT:  typeStr = "OT"; break;
		case OVC: typeStr = "OVC"; break;
		case OVG: typeStr = "OVG"; break;
		case OVN: typeStr = "OVN"; break;
		}
		C << "[" << typeStr << ", " << pOrd->TQ << ", " << pOrd->ID << "]";
	}
	return C;

}




void Order::SetAssignedChefID(int id)
{
	AssignedChefID = id;
}

int Order::GetAssignedChefID() const
{
	return AssignedChefID;
}

void Order::setAssignedChef(Chefs* pCh) { pAssignedChef = pCh; }
Chefs* Order::getAssignedChef() { return pAssignedChef; }
void Order::setAssignedTable(Table* pT) { pAssignedTable = pT; }
Table* Order::getAssignedTable() { return pAssignedTable; }
void Order::setAssignedScooter(Scooter* pS) { pAssignedScooter = pS; }
Scooter* Order::getAssignedScooter() { return pAssignedScooter; }