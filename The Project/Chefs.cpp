#include "Chefs.h"
#include <string>
#include<iostream>
using namespace std;

Chefs::Chefs(int id, CHEF_TYPE T, int S):ID(id) , Type(T) , Speed(S)
{
	Hold_Order = nullptr; //This meen the Chef is Avalable
}

void Chefs::SetHoldOrder(Order* O)
{
	Hold_Order = O;
}

int Chefs::Get_ID() const
{
	return ID;
}

CHEF_TYPE Chefs::Get_Type() const
{
	return Type;
}

int Chefs::Get_Speed() const
{
	return Speed;
}

Order* Chefs::Get_Current_Order() const
{
	return Hold_Order;
}

Chefs* Chefs::GetChef(int id)
{
	if (this->Get_ID() == id) {
		return this;
	}
	else return nullptr;

}

void Chefs::Assign_Order(Order* O)
{
	Hold_Order = O;
}



Order* Chefs::Finish_Order()
{
	Order* FO = Hold_Order;
	Hold_Order = nullptr;
	return FO;
}

bool Chefs::Is_Avalable() const
{
	//if (Hold_Order == nullptr) {
	//	return true;
	//}
	//else return false;
	return (Hold_Order == nullptr);
}

ostream& operator<<(ostream& C, Chefs* Chef)
{
	if (Chef != nullptr) {
		string The_Type = "";
		bool Avalable = Chef->Is_Avalable();
		//if (Chef->Get_Type() == CN)  The_Type = "CN";
		//else The_Type = "CS";

		The_Type = (Chef->Get_Type() == CN) ? "CN" : "CS";
		C << "[Chef , " << The_Type << " , " << Chef->Get_ID() << " , Avalable: " << Avalable << "]";
	}
	return C;
}
