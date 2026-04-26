#pragma once
#include "LinkedQueue.h"
#include "Action.h"
#include "LinkedPriorityQueue.h"
#include "Order.h"
#include "ArrayStack.h"
#include "Chefs.h"
#include "CancelQueue.h"
#include "TablePriQueue.h"
#include "Table.h"
#include "Scooter.h"
#include"CancelPriQueue.h"

class Restaurant
{
private:
	//Action Lists: 
	LinkedQueue<Action*>   Request;
	LinkedQueue<Action*>  Cancellation;  //For Phase 2


	//Pending Orders :

	//The Dine_in Orders Lists --> 2
	LinkedQueue<Order*> Pending_ODN;
	LinkedQueue<Order*> Pending_ODG;
	//The Takeaway Orders --> 1
	LinkedQueue<Order*> Pending_OT;
	//The Delivery Orders --> 3
	LinkedQueue<Order*> Pending_OVN;
	LinkedPriorityQueue<Order*>  Pending_OVG; //Grilled
	CancelQueue<Order*>  Pending_OVC;  //Cold orders

	//For Cancel Orders
	LinkedQueue<Order*> Cancelled_Orders;


	//The Chefs :
	LinkedQueue<Chefs*>  Chef_N;
	LinkedQueue<Chefs*>  Chef_S;


	//The Tables :
	TablePriQueue<Table*> Free_Tables;
	TablePriQueue<Table*> Busy_Sharable;
	TablePriQueue<Table*> Busy_No_Share;


	//The Ready Orders :
	LinkedQueue<Order*>  Ready_OD;
	LinkedQueue<Order*>  Ready_OT;
	CancelQueue<Order*>  Ready_OV;  //Available to cancel the Order


	//Finished Orders :
	ArrayStack<Order*>  Finished_O;


	//The Scooters :  -->3
	LinkedPriorityQueue<Scooter*> Free_Scot;
	LinkedPriorityQueue<Scooter*> Back_Scot;
	LinkedQueue<Scooter*> Maint_Scot;


	//Cooking Orders : 
	CancelPriQueue<Order*> Cook_O;


	//Insevise Orders :
	LinkedPriorityQueue<Order*>  Inserv_O;

	int TotalOrders;
public:

	//To Add Orders
	void AddOrder(Order* O);
	void AddCookOrder(ORD_TYPE O_Type,int CurrentTime);
	void CookingToReady(int CurrentTime);
	void ReadyToService(ORD_TYPE O_Type,int CurrentTime);
	void ServiceToFinish(int CurrentTime);

	void CancelOrder(int id,int Time);
	void CancelReadyOVC(int id);
	void CancelCookingOV(int id);

	//To Add the Chefs Lists
	void AddChef(int id, CHEF_TYPE type, int speed);

	//To Add the Tables
	void AddTable(int id, int capacity);

	void AddScooter(int id, int s, int maind, int maxorders);
	void HandleBackScooters(int CurrentTime);
	void MaintenanceToFree(int CurrentTime);

	//To print the information
	void Print_PendingOrders();
	void Print_CancelledOrders();
	void Print_CookingOrders();
	void Print_ReadyOrders();
	void Print_InServiceOrders();
	void Print_FinishedOrders();
	void Print_AvailableScooters();
	void PrintChefs();
	void PrintTables() const;

	template<typename T>
	void PrintIDs(LinkedQueue<T*>& List);

	template<typename T>
	void PrintChIDs(LinkedQueue<T*>& List);

	template<typename T>
	void PrintIDs(LinkedPriorityQueue<T*>& List);

	template<typename T>
	void PrintIDs(ArrayStack<T*>& List);

	bool AllOrdersFinished();

	~Restaurant();
	Restaurant();


};