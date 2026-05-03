#pragma once
#include "Action.h"
#include"Restaurant.h"
class CancelAction :public Action
{
private:
    int OrderID;

public:
    CancelAction(int id, int time)
    {
        OrderID = id;
        actionTime = time;
    }

    virtual ~CancelAction() {}

    virtual void Act(Restaurant* pRest) override
    {
        pRest->CancelOrder(OrderID,actionTime);
    }

};