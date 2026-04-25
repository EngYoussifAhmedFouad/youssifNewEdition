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
    

public:
    RequestAction(int id, ORD_TYPE typ, int sz, double mon, int time)
    {
        OrderID = id;
        type = typ;
        size = sz;
        money = mon;
        actionTime = time;
    }

    virtual ~RequestAction() {}

    virtual void Act(Restaurant* pRest) override
    {
         Order* pOrd = new Order(OrderID, type, size, money, actionTime);
         pRest->AddOrder(pOrd); 
         
    }
};