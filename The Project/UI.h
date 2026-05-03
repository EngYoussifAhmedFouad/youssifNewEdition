#pragma once
#include "LinkedQueue.h"
#include "Table.h"
#include "Chefs.h"
#include "Scooter.h"
#include <iostream>
using namespace std;

// Forward declaration to avoid circular include
class Restaurant;

class UI
{
private:
	int CurrentTime;
public:
	UI();

	void PrintCurrentTime(int cur_Time) const;

	void PrintAll(Restaurant* pRest);

	//To take the lood file name as string
	string Getfilename()const;

	//void waitForClick();

	/*void PrintTables(LinkedQueue<Table*>& Tables) const;
	void PrintAvailableChefs(const LinkedQueue<Chefs*>& Chefs) const;
	void PrintAvailableScooters(LinkedQueue<Scooter*>& Scooters) const;
	void PrintInMainScooters(LinkedQueue<Scooter*>& Scooters) const;
	void PrintBackScooters(LinkedQueue<Scooter*>& Scooters) const;
	void PrintPendingOrders(Restaurant*& Res) const*/

	~UI();
};

