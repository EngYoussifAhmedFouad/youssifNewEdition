#include "UI.h"
#include <iostream>
using namespace std;

UI::UI() : CurrentTime(0) {}

void UI::PrintCurrentTime(int cur_Time) const {
    cout << "Current Timestep: " << cur_Time << endl;
}

UI::~UI() {}

void UI::PrintAll(Restaurant* pRest) {
    pRest->Print_PendingOrders();
    pRest->PrintChefs();
    pRest->Print_CookingOrders();
    pRest->Print_ReadyOrders();
    pRest->Print_AvailableScooters();
    pRest->PrintTables();
    pRest->Print_InServiceOrders();
    //pRest->Print_InMaintenance();
    //pRest->Print_ScootersBack();
    pRest->Print_CancelledOrders();
    pRest->Print_FinishedOrders();

    cout << "\nPRESS ANY KEY TO MOVE TO NEXT STEP !";
    // Standard pause logic
    cin.clear();
    cin.ignore(100, '\n');
    cin.get();
}