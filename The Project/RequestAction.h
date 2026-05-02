#pragma once
#include "Action.h"
#include "GenericDefs.h"

#include"Order.h"

class RequestAction : public Action
{
private:
    int OrderID;
    ORD_TYPE type;
    int size;
    double money;
	bool CanShare;
	int Seats;
	int Duration;
	int Distance;
    Order* pOrd;
public:
    RequestAction(int id, ORD_TYPE typ, int sz, double mon, int time)
    {
        Order* pOrd = new Order(OrderID, type, size, money, actionTime);
        OrderID = id;
        type = typ;
        size = sz;
        money = mon;
        actionTime = time;
    }
    RequestAction(int id, ORD_TYPE typ, int sz, double mon, int time, int seats, int duration,bool canShare)
    {
        Order* pOrd = new Order(OrderID, type, size, money, actionTime);
        OrderID = id;
        type = typ;
        size = sz;
        money = mon;
        actionTime = time;
		Duration = duration;
		CanShare = canShare;
    }
    RequestAction(int id, ORD_TYPE typ, int sz, double mon, int time, int distance)
    {
        Order* pOrd = new Order(OrderID, type, size, money, actionTime);
		OrderID = id;
		type = typ;
		size = sz;
		money = mon;
		actionTime = time;
		Distance = distance;
    }
    virtual ~RequestAction() {}

    virtual void Act(Restaurant* pRest) override
    {
        
         pRest->AddOrder(pOrd); 
         
    }
};