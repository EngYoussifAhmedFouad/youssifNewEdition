#pragma once
#include "Restaurant.h"
//class Restaurant;
class Action
{

protected:
	int actionTime;
public:
	Action()
	{
	}
	virtual ~Action()
	{
	}

	virtual void Act(Restaurant* pRest) = 0;
};

