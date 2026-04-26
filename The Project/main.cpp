#include <iostream>
#include <cstdlib>
#include <ctime> 
#include "Restaurant.h"
#include "UI.h"

using namespace std;

int main() {
    srand(static_cast<unsigned int>(time(NULL)));

    Restaurant* myRestaurant = new Restaurant();
    UI ui;

    for (int i = 1; i <= 20; i++) myRestaurant->AddChef(i, CN, 5);
    for (int i = 21; i <= 40; i++) myRestaurant->AddChef(i, CS, 5);

    for (int i = 1; i <= 10; i++) myRestaurant->AddTable(i, 6);

    for (int i = 1; i <= 15; i++) myRestaurant->AddScooter(i, 20, 5, 3);

    cout << "Generating 500 Random Pending Orders..." << endl;
    for (int i = 1; i <= 500; i++) {
        ORD_TYPE type = static_cast<ORD_TYPE>(rand() % 6);
        int size = (rand() % 10) + 1;
        double price = (rand() % 1000) + 100;
        int tq = 1;

        Order* newOrd = new Order(i, type, size, price, tq);
        myRestaurant->AddOrder(newOrd);
    }

    int currentTime = 1;

    while (!myRestaurant->AllOrdersFinished()) {
        ui.PrintCurrentTime(currentTime);

        for (int i = 0; i < 30; i++) {
            ORD_TYPE randType = static_cast<ORD_TYPE>(rand() % 6);
            myRestaurant->AddCookOrder(randType,currentTime);
        }

        if ((rand() % 100) < 75) {
            for (int i = 0; i < 15; i++) {
                myRestaurant->CookingToReady(currentTime);
            }
        }

        for (int i = 0; i < 10; i++) {
            ORD_TYPE randType = static_cast<ORD_TYPE>(rand() % 6);
            myRestaurant->ReadyToService(randType, currentTime);
        }

        for (int k = 0; k < 50; k++) {
            int cancelID = (rand() % 500) + 1;
            if ((rand() % 100) < 30) {
                myRestaurant->CancelOrder(cancelID, currentTime);
            }
            if ((rand() % 100) < 30) {
                myRestaurant->CancelReadyOVC(cancelID);
            }
            if ((rand() % 100) < 30) {
                myRestaurant->CancelCookingOV(cancelID);
            }
        }

        if ((rand() % 100) < 25) {
            myRestaurant->ServiceToFinish(currentTime);
        }

        if ((rand() % 100) < 50) {
            myRestaurant->HandleBackScooters(currentTime);
        }

        if ((rand() % 100) < 50) {
            myRestaurant->MaintenanceToFree(currentTime);
        }

        ui.PrintAll(myRestaurant);

        cout << "\nPRESS ENTER TO MOVE TO NEXT STEP !";

        cin.clear();
        if (currentTime == 1) cin.ignore(1000, '\n');
        cin.get();

        currentTime++;
    }

    cout << "\nSimulation Finished! All 500 orders processed." << endl;

    delete myRestaurant;
    return 0;
}