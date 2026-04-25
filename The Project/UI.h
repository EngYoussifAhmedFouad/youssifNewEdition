#pragma once
#include "LinkedQueue.h"
#include "Table.h"
#include "Chefs.h"
#include "Scooter.h"
#include "Restaurant.h"
#include <iostream>
using namespace std;
class UI
{
private:
	int CurrentTime;
public:
	UI();

	void PrintCurrentTime(int cur_Time) const;

	void PrintAll(Restaurant* pRest);

	//void waitForClick();

	/*void PrintTables(LinkedQueue<Table*>& Tables) const;
	void PrintAvailableChefs(const LinkedQueue<Chefs*>& Chefs) const;
	void PrintAvailableScooters(LinkedQueue<Scooter*>& Scooters) const;
	void PrintInMainScooters(LinkedQueue<Scooter*>& Scooters) const;
	void PrintBackScooters(LinkedQueue<Scooter*>& Scooters) const;
	void PrintPendingOrders(Restaurant*& Res) const*/

	~UI();
};

