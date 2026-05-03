#pragma once
//#include "Restaurant.h"
class Restaurant;
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

	int GetActiontime()const {
		return actionTime;
	}

	virtual void Act(Restaurant* pRest) = 0;
};

