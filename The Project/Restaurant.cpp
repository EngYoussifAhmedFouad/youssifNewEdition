#include "Restaurant.h"
#include"RequestAction.h"
#include"CancelAction.h"
#include<cmath>
#include <fstream>
#include <iostream>
using namespace std;

Restaurant::Restaurant() : TotalOrders(0) {
    pUI = new UI();
}

void Restaurant::AddOrder(Order* O) {
    TotalOrders++;
    ORD_TYPE type = O->Get_Type();
    switch (type) {
    case ODN: Pending_ODN.enqueue(O); break;
    case ODG: Pending_ODG.enqueue(O); break;
    case OT:  Pending_OT.enqueue(O);  break;
    case OVN: Pending_OVN.enqueue(O); break;
    case OVC: Pending_OVC.enqueue(O); break;
    case OVG: {
        int priority = (O->Get_Price() * 10) - (O->Get_Distance() * 2) - (O->Get_Size() * 1) - (O->Get_TQ() * 5);
        Pending_OVG.enqueue(O, priority);
        break;
    }
    }
}

void Restaurant::CancelOrder(int id,int Time)
{
    Order* pCancelledOrder = nullptr;

    bool isFound = Pending_OVC.CancelOrder(id, pCancelledOrder);

    if (isFound && pCancelledOrder != nullptr) {

		pCancelledOrder->SetTCancel(Time);
        Cancelled_Orders.enqueue(pCancelledOrder);
        return;
    }

    return;
}

void Restaurant::CancelReadyOVC(int id) {
    Order* pOrd = nullptr;
    // Assuming Ready_OV is derived to support CancelOrder(id)
    if (Ready_OV.CancelOrder(id, pOrd)) {
        Cancelled_Orders.enqueue(pOrd);
    }
}

void Restaurant::CancelCookingOV(int id) {
    Order* pOrd = nullptr;
    // Assuming Cook_O is derived to support CancelOrder(id)
    if (Cook_O.CancelOrder(id, pOrd)) {
        // Release the Chef link before cancelling
		if (pOrd->getAssignedChef()) 
        {
			pOrd->getAssignedChef()->Finish_Order(); // Release Chef
			pOrd->setAssignedChef(nullptr); // Clear Chef assignment
		}
        Cancelled_Orders.enqueue(pOrd);
    }
}

void Restaurant::AddChef(int id, CHEF_TYPE type, int speed) {

    Chefs* Chef = new Chefs(id, type, speed);

    if (type == CN) {
        Chef_N.enqueue(Chef);
    }
    else {
        Chef_S.enqueue(Chef);
    }
}

void Restaurant::AddTable(int id, int capacity) {

    Table* Table_A = new Table(id, capacity);

    Free_Tables.enqueue(Table_A, -capacity);
}

void Restaurant::AddScooter(int id, int speed, int maindur, int maxorders)
{
    Scooter* Scooter_A = new Scooter(id, speed, maindur, maxorders);
    if (Scooter_A->getStatus() == "Available")
        Free_Scot.enqueue(Scooter_A, -(Scooter_A->getTotalDist()));
    else if (Scooter_A->getStatus() == "Back")
        Back_Scot.enqueue(Scooter_A, -(Scooter_A->getTotalDist()));
    else if (Scooter_A->NeedMain())
        Maint_Scot.enqueue(Scooter_A);

}

// 3.1: Assign to Chefs
void Restaurant::AddCookOrder(ORD_TYPE O_Type,int CurrentTime) {
    Order* O = nullptr;
    // Dequeue from Pending
    switch (O_Type) {
    case ODG: Pending_ODG.dequeue(O); break;
    case ODN: Pending_ODN.dequeue(O); break;
    case OT:  Pending_OT.dequeue(O);  break;
    case OVC: Pending_OVC.dequeue(O); break;
    case OVG: Pending_OVG.dequeue(O); break;
    case OVN: Pending_OVN.dequeue(O); break;
    }

    if (!O) return;

    Chefs* Ch = nullptr;
    bool assigned = false;

    // Assignment Logic: ODG/OVG use Super Chefs only. Others use Normal first.
    if (O_Type == ODG || O_Type == OVG) {
        int count = Chef_S.GetCount();
        for (int i = 0; i < count; i++) {
            Chef_S.dequeue(Ch);
            if (Ch->Is_Avalable() && !assigned) {
                Ch->Assign_Order(O); O->setAssignedChef(Ch); O->SetAssignedChefID(Ch->Get_ID());
				O->SetTA(CurrentTime);
				O->SetTR(CurrentTime + ceil((float)O->Get_Size() / Ch->Get_Speed()));
                
                Cook_O.enqueue(O,-(O->Get_TR())); 
                assigned = true;
            }
            Chef_S.enqueue(Ch);
        }
    }
    else {
        int count = Chef_N.GetCount();
        for (int i = 0; i < count; i++) {
            Chef_N.dequeue(Ch);
            if (Ch->Is_Avalable() && !assigned) {
                Ch->Assign_Order(O); O->setAssignedChef(Ch); O->SetAssignedChefID(Ch->Get_ID());

                O->SetTA(CurrentTime);
				O->SetTR(CurrentTime + ceil((float)O->Get_Size() / Ch->Get_Speed()));

                Cook_O.enqueue(O,-(O->Get_TR())); assigned = true;
            }
            Chef_N.enqueue(Ch);
        }
        if (!assigned) {
            count = Chef_S.GetCount();
            for (int i = 0; i < count; i++) {
                Chef_S.dequeue(Ch);
                if (Ch->Is_Avalable() && !assigned) {
                    Ch->Assign_Order(O); O->setAssignedChef(Ch); O->SetAssignedChefID(Ch->Get_ID());
                    
                    O->SetTA(CurrentTime);
                    O->SetTR(CurrentTime + ceil((float)O->Get_Size() / Ch->Get_Speed()));
                 
                    Cook_O.enqueue(O,-(O->Get_TR())); assigned = true;
                }
                Chef_S.enqueue(Ch);
            }
        }
    }
    if (!assigned) { TotalOrders--; AddOrder(O); } // Put back if no chef
}

// 3.2: Move to Ready
void Restaurant::CookingToReady(int CurrentTime) {
    Order* O = nullptr;
    CancelQueue<Order*> Temp_Cook;
    while (Cook_O.peek(O))
    {
		
        if (O->Get_TR() == CurrentTime)
        {
			Cook_O.dequeue(O);
            if (O->getAssignedChef()) O->getAssignedChef()->Finish_Order(); // Release Chef
            ORD_TYPE type = O->Get_Type();
            if (type == ODN || type == ODG) Ready_OD.enqueue(O);
            else if (type == OT) Ready_OT.enqueue(O);
            else Ready_OV.enqueue(O);
        }
        else
        {
            break;
        }
    }
}

// 3.3: Ready to In-Service (Assign Scooters/Tables)
void Restaurant::ReadyToService(ORD_TYPE O_Type,int CurrentTime) {
    Order* O = nullptr;
    switch (O_Type) {
    case ODG: case ODN: Ready_OD.dequeue(O); break;
    case OT: Ready_OT.dequeue(O); break;
    case OVC: case OVG: case OVN: Ready_OV.dequeue(O); break;
    }

    if (!O) return;
   
    // OT goes straight to finished
    if (O_Type == OT) 
    {
		O->SetTF(CurrentTime);
        Finished_O.push(O);
        return; 
    } 

    bool assigned = false;
    if (O_Type == ODG || O_Type == ODN) 
    { // Dine-in needs Table
        Table* T = nullptr;
        int count = Free_Tables.GetCount();
        for (int i = 0; i < count; i++) {
            Free_Tables.dequeue(T);
            if (T->AssignTable(O)) { O->setAssignedTable(T);  assigned = true; break; }
            Free_Tables.enqueue(T, -(T->getCapacity()));
        }
		if (assigned)
        {
            O->SetTF(CurrentTime + O->Get_Duration());
            Inserv_O.enqueue(O, -(O->Get_Tf()));
        }
    }
    else { // Delivery needs Scooter
        Scooter* S = nullptr;
        if (Free_Scot.dequeue(S)) { 
            S->AssignScooter(O); 
            O->setAssignedScooter(S); 
			// Calculate delivery time and set times
            O->SetTS(CurrentTime);
			int deliveryTime = ceil(O->Get_Distance() / S->getSpeed());
			O->SetTF(CurrentTime + deliveryTime);

            Inserv_O.enqueue(O, -(O->Get_Tf()));
            assigned = true; 
        }
    }

    if (!assigned) { // Return to ready if no resource
        if (O_Type == ODG || O_Type == ODN) Ready_OD.enqueue(O);
        else Ready_OV.enqueue(O);
    }
}

// 3.7: Finish In-Service
void Restaurant::ServiceToFinish(int CurrentTime) 
{
    Order* O = nullptr;
    while(!Inserv_O.isEmpty())
    {
        Inserv_O.peek(O);
        if (O->Get_Tf() == CurrentTime) {
            Inserv_O.dequeue(O);
            Finished_O.push(O);
            if (O->getAssignedScooter()) {
                O->getAssignedScooter()->FinishDel();
                Back_Scot.enqueue(O->getAssignedScooter(), -(O->getAssignedScooter()->getTotalDist()));
            }
            else if (O->getAssignedTable()) {
				Table* T = O->getAssignedTable();
				T->releasetable(O->Get_Seats()); // Add seats back

                if(T->getFreeSeats()==T->getCapacity())
                {
                    Free_Tables.enqueue(T, -(T->getCapacity()));
                }
            }
        }
        else
        {
            break;
        }
    }
}

void Restaurant::HandleBackScooters(int CurrentTime) {
    Scooter* S = nullptr;
    while (Back_Scot.dequeue(S)) {
        if (S->NeedMain()) { S->SendToMain(); S->SetReturnTime(CurrentTime+S->getMainDuration()); Maint_Scot.enqueue(S); }
        else { S->Return(); Free_Scot.enqueue(S, -(S->getTotalDist())); }
    }
}

void Restaurant::MaintenanceToFree(int CurrentTime) {
    Scooter* S = nullptr;

    while(Maint_Scot.peek(S))
    {
        if (S->getReturnTime() <= CurrentTime)
        {
			Maint_Scot.dequeue(S);
            S->FinishMain();
            Free_Scot.enqueue(S, -(S->getTotalDist()));

        }
        else
        {
			break; 
        }
    }
}

// --- PRINTING FUNCTIONS ---


void Restaurant::Print_CancelledOrders()
{
    cout << "----------Cancelled Orders----------" << endl;
    cout << Cancelled_Orders.GetCount() << " X: ";
    PrintIDs(Cancelled_Orders);
}

void Restaurant::Print_CookingOrders()
{
    cout << "----------Cooking Orders----------" << endl;
    cout << Cook_O.GetCount() << " Cooking Orders: ";
    PriNode<Order*>* currNode = Cook_O.getFront();

    bool first = true;

    while (currNode != nullptr)
    {
        Order* O = currNode->getItem();

        if (O != nullptr)
        {
            if (!first) cout << ", ";
            cout << "[" << O->Get_ID() << "," << O->GetAssignedChefID() << "]";
            first = false;
        }

        currNode = currNode->getNext();
    }
    cout << endl;
}

void Restaurant::Print_ReadyOrders()
{
    cout << "----------Ready Orders----------" << endl;

    cout << Ready_OD.GetCount() << " OD: ";
    PrintIDs(Ready_OD);

    cout << Ready_OT.GetCount() << " OT: ";
    PrintIDs(Ready_OT);

    cout << Ready_OV.GetCount() << " OV: ";
    PrintIDs(Ready_OV);

}

void Restaurant::Print_FinishedOrders()
{
    cout << "----------Finished Orders----------" << endl;
    cout << endl <<Finished_O.GetCount() << " Orders: ";
    PrintIDs(Finished_O);
}

void Restaurant::Print_AvailableScooters()
{
    cout << endl << "----------Available Scooters IDs----------" << endl;
    cout << Free_Scot.GetCount() << " Scooters: ";
    PrintIDs(Free_Scot);
}



void Restaurant::Print_PendingOrders() {
    int total = Pending_ODN.GetCount() + Pending_ODG.GetCount() + Pending_OT.GetCount() + Pending_OVN.GetCount() + Pending_OVC.GetCount() + Pending_OVG.GetCount();
    cout << "--------------------- Pending Orders IDs ----------------------" << endl;
    cout << total << " Pending orders: " << endl;
    if (Pending_ODN.GetCount() > 0) { cout << Pending_ODN.GetCount() << " ODN: "; PrintIDs(Pending_ODN); cout << " " << endl; }
    if (Pending_ODG.GetCount() > 0) { cout << Pending_ODG.GetCount() << " ODG: "; PrintIDs(Pending_ODG); cout << " " << endl; }
    if (Pending_OT.GetCount() > 0) { cout << Pending_OT.GetCount() << " OT: "; PrintIDs(Pending_OT); cout << " " << endl; }
    if (Pending_OVN.GetCount() > 0) { cout << Pending_OVN.GetCount() << " OVN: "; PrintIDs(Pending_OVN); cout << " " << endl; }
    if (Pending_OVC.GetCount() > 0) { cout << Pending_OVC.GetCount() << " OVC: "; PrintIDs(Pending_OVC); cout << " " << endl; }
    if (Pending_OVG.GetCount() > 0) { cout << Pending_OVG.GetCount() << " OVG: "; PrintIDs(Pending_OVG); cout << endl; }
    cout << endl;
}

void Restaurant::PrintChefs() {
    cout << "--------------------- Available chefs IDs ---------------------" << endl;
    // Note: Manual filter to ensure we only print IDs of chefs without Hold_Order
    cout << Chef_S.GetCount() << " CS: "; PrintChIDs(Chef_S); cout << Chef_N.GetCount() << "  CN: "; PrintChIDs(Chef_N); cout << endl;
}

void Restaurant::PrintTables() const
{
    PriNode<Table*>* currNode = Free_Tables.getFront();
    cout << "----------Available Tables----------" << endl;
    cout << Free_Tables.GetCount() << " Tables: ";

    bool first = true;

    while (currNode != nullptr)
    {
        Table* T = currNode->getItem();

        if (T != nullptr)
        {
            if (!first) cout << ", ";
            T->Print();
            first = false;
        }

        currNode = currNode->getNext();
    }
    cout << endl;
}

bool Restaurant::Loadfile()
{
    string name = pUI->Getfilename();

    ifstream inputFile(name);
    
    if (!inputFile.is_open()) return false; 


    //Chefs
    int Cn, Cs, speedN, speedS;
    inputFile >> Cn >> Cs >> speedN >> speedS;
    for (int i = 1; i <= Cn; i++) {
        AddChef(i, CN, speedN);
        
    }
    for (int i = 1; i <= Cs; i++) {
        AddChef(i, CS, speedS);
    }

    //Scooters
    int Scooter, SpeedSr, Main_Ords, Main_Dur;
    inputFile >> Scooter >> SpeedSr >> Main_Ords >> Main_Dur;
    for (int i = 1; i <= Scooter; i++) {
        AddScooter(i, SpeedSr, Main_Dur, Main_Ords);
    }

    //Tables
    int totalTables;
    int CurrentId=1;//For Seting the Id's
    inputFile >> totalTables;
    while(totalTables!=0){
        int Count, capacity;
        inputFile >> Count >> capacity;
        for (int j = 0; j < Count; j++) {
            AddTable(CurrentId, capacity);
            CurrentId++;
        }
        totalTables = totalTables - Count;

    }

    //Actions
    int numActions;
    inputFile >> numActions;
    for (int i = 0; i < numActions; i++) {
        char actionType;
        inputFile >> actionType;

        if (actionType == 'Q') { // Request Action
            string type;
            ORD_TYPE typeA;
            int TQ, ID, Size, Price;
            //For OD
            int Seats, Duration;
            bool CanShare;
            //For Ov
            float distance;
            inputFile >> type;

            if (type == "ODG") typeA = ODG;
            else if (type == "ODN") typeA = ODN;
            else if (type == "OT") typeA = OT;
            else if (type == "OVC") typeA = OVC;
            else if (type == "OVG") typeA = OVG;
            else if (type == "OVN") typeA = OVN;
            //else if (type == "X") typeA = X;

            inputFile >> type >> TQ >> ID >> Size >> Price;
            if (typeA == ODN || typeA == ODG) inputFile >> Seats >> Duration >> CanShare;
            if (typeA == OVC || typeA == OVG || typeA == OVN) inputFile >> distance;
            Action* pAct;
            if (typeA == ODN || typeA == ODG) pAct = new RequestAction(ID, typeA, Size, Price, TQ, Seats, Duration, CanShare);
            else if (typeA == OVC || typeA == OVG || typeA == OVN) pAct = new RequestAction(ID, typeA, Size, Price, TQ, distance);
            else pAct = new RequestAction(ID, typeA, Size, Price, TQ);
            
            ActionsList.enqueue(pAct);



        }
        else if (actionType == 'X') { // Cancel Action
            int Tcancel, ID;
            inputFile >> Tcancel >> ID;
            Action* pAct = new CancelAction(ID, Tcancel);
            ActionsList.enqueue(pAct);
        }
    }

    inputFile.close();
    return true;

}

void Restaurant::Simulation()
{
    
    string fileName = pUI->Getfilename();
    if (!Loadfile()) return;//If the file Not open

    int CurrentTimeStep = 1;
    bool IsSimulation = true;


}

void Restaurant::ExecuteCurrentActions(int CT)
{
    Action* pAct;
    while (ActionsList.peek(pAct)) {
        if (pAct->GetActiontime() == CT) {
            ActionsList.dequeue(pAct);
            pAct->Act(this); 
        }
        else {
            break; 
        }
    }

}

void Restaurant::CheckScooters(int CT)
{
    Scooter* pS;

    while (Maint_Scot.peek(pS)) {
        if (pS->getReturnTime() <= CT) {
            Maint_Scot.dequeue(pS); 
            pS->FinishMain(); 
            Free_Scot.enqueue(pS, pS->getTotalDist());
        }
        else {
            break; 
        }
    }

    while (Back_Scot.peek(pS)) {
        if (pS->getReturnTime() <= CT) {
            Back_Scot.dequeue(pS); 

            if (pS->NeedMain()) {
                pS->SetReturnTime(CT + pS->getMainDuration());
                Maint_Scot.enqueue(pS);
            }
            else {
                Free_Scot.enqueue(pS, pS->getTotalDist());
            }
        }
        else {
            break;
        }
    }

}

void Restaurant::CheckInserve_OD(int CT)
{
    Order* pOrd;

    while (Inserv_O.peek(pOrd)) {

        if (pOrd->Get_Tf() <= CT) {
            Inserv_O.dequeue(pOrd);

            Table* pT = pOrd->getAssignedTable();
            if (pT) {
                pT->resettable(); 
                Free_Tables.enqueue(pT, -1*(pT->getFreeSeats()));
            }

            
            Finished_O.push(pOrd);

        }
        else {
            break; 
        }
    }

}

void Restaurant::AssignOrders(int CT) {

    AssignPendingToChefs(CT);

    AssignReadyToResources(CT);
}

void Restaurant::AssignPendingToChefs(int CT)
{

    Order* pOrd = nullptr;
    Chefs* pChef;

    while (Pending_ODG.peek(pOrd) || Pending_OVG.peek(pOrd)) {
        if (Chef_S.dequeue(pChef) && pChef->Is_Avalable() ) {
            if (pOrd->Get_Type() == ODG) {
                Pending_ODG.dequeue(pOrd);
                pChef->Assign_Order(pOrd);
                pOrd->setAssignedChef(pChef);
            }
            if (pOrd->Get_Type() == OVG) {
                Pending_OVG.dequeue(pOrd);
                pChef->Assign_Order(pOrd);
                pOrd->setAssignedChef(pChef);
            }
            Cook_O.enqueue(pOrd, pOrd->Get_Tc());
        }
        else {
            break;
        }
    }

    while (Pending_ODN.peek(pOrd) || Pending_OT.peek(pOrd) || Pending_OVN.peek(pOrd) || Pending_OVC.peek(pOrd)){
        if (Chef_N.dequeue(pChef) ) {
            if (pOrd->Get_Type() == ODN) {
                Pending_ODN.dequeue(pOrd);
                
            }else if (pOrd->Get_Type() == OT) {
                Pending_OT.dequeue(pOrd);
                
            }
            else if (pOrd->Get_Type() == OVN) {
                Pending_OVN.dequeue(pOrd);
            }
            else if (pOrd->Get_Type() == OVC) {
                Pending_OVC.dequeue(pOrd);
            }
            pOrd->SetTA(CT);
            pChef->Assign_Order(pOrd);
            pOrd->setAssignedChef(pChef);

            Cook_O.enqueue(pOrd, pOrd->Get_Tc());
        }
        //if we not found free normal chefs
        else if (Chef_S.dequeue(pChef)) {
            if (pOrd->Get_Type() == ODN) {
                Pending_ODN.dequeue(pOrd);

            }
            else if (pOrd->Get_Type() == OT) {
                Pending_OT.dequeue(pOrd);

            }
            else if (pOrd->Get_Type() == OVN) {
                Pending_OVN.dequeue(pOrd);
            }
            else if (pOrd->Get_Type() == OVC) {
                Pending_OVC.dequeue(pOrd);
            }
            pOrd->SetTA(CT);
            pChef->Assign_Order(pOrd);
            pOrd->setAssignedChef(pChef);

            Cook_O.enqueue(pOrd, pOrd->Get_Tc());
        }else{
            break;
        }
    }


}

void Restaurant::AssignReadyToResources(int CT)
{
    //For OD
    AssignTablesTo_OD(CT);
    //For OV
    AssignScootersTo_OV(CT);
    //For OT
    FinalizeTakeawayOrders(CT);



}

void Restaurant::AssignTablesTo_OD(int CT)
{
    // Minimal implementation: attempt to assign ready dine-in orders to tables.
    // For now, keep behavior simple to avoid changing existing logic — leave orders in Ready_OD if not handled elsewhere.
    // This function can be expanded later with allocation logic.
    (void)CT; // suppress unused parameter warning
}

void Restaurant::AssignScootersTo_OV(int CT)
{
    // Minimal implementation placeholder for assigning scooters to ready delivery orders.
    (void)CT;
}

void Restaurant::FinalizeTakeawayOrders(int CT)
{
    // Minimal implementation for finalizing OT orders (move to finished if required).
    (void)CT;
}

void Restaurant::Print_InServiceOrders() {
    cout << "----------- In-Service orders [order ID, scooter/Table ID] -----------" << endl;
    cout << Inserv_O.GetCount() << " Orders: ";
    PriNode<Order*>* curr = Inserv_O.getFront();
    bool first = true;
    while (curr) {
        Order* O = curr->getItem();
        if (!first) cout << ", ";
        cout << "[" << O->Get_ID() << ", ";
        if (O->getAssignedScooter()) cout << "S" << O->getAssignedScooter()->Get_ID();
        else if (O->getAssignedTable()) cout << "T" << O->getAssignedTable()->Get_ID();
        cout << "]";
        first = false; curr = curr->getNext();
    }
    cout << endl;
}

// Template helper (Shared across lists)
/*template<typename T>
void Restaurant::PrintIDs(LinkedQueue<T*>& List) {
    Node<T*>* curr = List.getFront();
    bool first = true;
    while (curr) {
        if (!first) cout << ", ";
        cout << curr->getItem()->Get_ID();
        first = false; curr = curr->getNext();
    }
}*/

template<typename T>
void Restaurant::PrintIDs(LinkedQueue<T*>& List)
{
    Node<T*>* currNode = List.getFront();

    bool first = true;

    while (currNode != nullptr)
    {
        T* O = currNode->getItem();

        if (O != nullptr)
        {
            if (!first) cout << ", ";
            cout << O->Get_ID();
            first = false;
        }

        currNode = currNode->getNext();
    }
    cout << endl;
}

template<typename T>
void Restaurant::PrintChIDs(LinkedQueue<T*>& List)
{
    Node<Chefs*>* currNode = List.getFront();

    bool first = true;

    while (currNode != nullptr)
    {
        Chefs* O = currNode->getItem();

        if (O != nullptr)
        {
            if (!first) cout << "  ";
            if (O->Is_Avalable())
                cout << O->Get_ID();
            first = false;
        }

        currNode = currNode->getNext();
    }
    cout << endl;
}

template<typename T>
void Restaurant::PrintIDs(LinkedPriorityQueue<T*>& List)
{
    PriNode<T*>* currNode = List.getFront();

    bool first = true;

    while (currNode != nullptr)
    {
        T* O = currNode->getItem();

        if (O != nullptr)
        {
            if (!first) cout << ", ";
            cout << O->Get_ID();
            first = false;
        }

        currNode = currNode->getNext();
    }
    cout << endl;
}

template <typename T>
void Restaurant::PrintIDs(ArrayStack<T*>& List)
{
    ArrayStack<T*> temp;
    T* pObj = nullptr;

    bool first = true;

    while (List.pop(pObj))
    {
        if (pObj != nullptr)
        {
            if (!first) cout << ", ";
            cout << pObj->Get_ID();
            first = false;
        }

        temp.push(pObj); // save it
    }

    // restore original stack
    while (temp.pop(pObj))
    {
        List.push(pObj);
    }

    cout << endl;
}
// Add similar overloads for PriQueue and Stack...

bool Restaurant::AllOrdersFinished() {
    return (Finished_O.GetCount() + Cancelled_Orders.GetCount() >= TotalOrders && TotalOrders > 0);
}

Restaurant::~Restaurant()
{
    Order* pOrd;
    Chefs* pChef;
    Action* pAct;
    Table* pTable;
    Scooter* pScooter;

    //Delete the Actions
    while (ActionsList.dequeue(pAct)) { delete pAct; }

    //Delete the Chefs
    while (Chef_N.dequeue(pChef)) { delete pChef; }
    while (Chef_S.dequeue(pChef)) { delete pChef; }

    //Delete the tables
    while (Free_Tables.dequeue(pTable)) { delete pTable; }
    while (Busy_Sharable.dequeue(pTable)) { delete pTable; }
    while (Busy_No_Share.dequeue(pTable)) { delete pTable; }

    //Delete the Scooter
    while (Free_Scot.dequeue(pScooter)) { delete pScooter; }
    while (Back_Scot.dequeue(pScooter)) { delete pScooter; }
    while (Maint_Scot.dequeue(pScooter)) { delete pScooter; }

    //Delete The All Orders

    // 1 - Pending Orders
    while (Pending_ODN.dequeue(pOrd)) { delete pOrd; }
    while (Pending_ODG.dequeue(pOrd)) { delete pOrd; }
    while (Pending_OT.dequeue(pOrd)) { delete pOrd; }
    while (Pending_OVN.dequeue(pOrd)) { delete pOrd; }
    while (Pending_OVG.dequeue(pOrd)) { delete pOrd; }
    while (Pending_OVC.dequeue(pOrd)) { delete pOrd; }

    // 2 - Cooking & In-Service
    while (Cook_O.dequeue(pOrd)) { delete pOrd; }
    while (Inserv_O.dequeue(pOrd)) { delete pOrd; }

    // 3 - Ready Orders
    while (Ready_OD.dequeue(pOrd)) { delete pOrd; }
    while (Ready_OT.dequeue(pOrd)) { delete pOrd; }
    while (Ready_OV.dequeue(pOrd)) { delete pOrd; }

    // 4 - Finished Orders 
    while (Finished_O.pop(pOrd)) { delete pOrd; }




}