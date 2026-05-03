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
    int money;
    bool CanShare;
    int Seats;
    int Duration;
    float Distance;
    Order* pOrd;
public:
    RequestAction(int id, ORD_TYPE typ, int sz, double mon, int time)
    {
        
        OrderID = id;
        type = typ;
        size = sz;
        money = mon;
        actionTime = time;
        pOrd = new Order(OrderID, type, size, money, actionTime);
    }
    RequestAction(int id, ORD_TYPE typ, int sz, double mon, int time, int seats, int duration, bool canShare):RequestAction(id, typ, sz, mon, time)
    {
        Duration = duration;
        CanShare = canShare;
        Seats = seats;
        pOrd->Set_Seats(Seats);
        pOrd->Set_Duration(Duration);
        pOrd->Set_Can_Share(CanShare);

    }
    RequestAction(int id, ORD_TYPE typ, int sz, double mon, int time, int distance):RequestAction(id, typ, sz, mon, time)
    {
        Distance = distance;
        pOrd->Set_Distance(Distance);
    }
    virtual ~RequestAction() {}

    virtual void Act(Restaurant* pRest) override
    {

        pRest->AddOrder(pOrd);

    }
};