#include <iostream>
#include <cstdlib> // عشان دالة rand() و srand()
#include <ctime>   // عشان دالة time()
#include "Restaurant.h"
#include "Order.h"

// متنساش تعمل include لملفات الـ Actions بتاعتك هنا
// #include "RequestAction.h"
// #include "CancelAction.h"

using namespace std;

int main() {
    cout << "===========================================" << endl;
    cout << "   Starting Phase 1.2 Random Simulator     " << endl;
    cout << "===========================================" << endl << endl;

    // 1. تغيير بذرة الأرقام العشوائية عشان الأرقام تتغير مع كل مرة تعمل فيها Run
    srand(time(NULL));

    // 2. إنشاء المطعم (المايسترو)
    Restaurant* myRestaurant = new Restaurant();

    // 3. محاكاة توليد 500 حدث عشوائي
    cout << "Generating 500 Random Events (Requests & Cancellations)..." << endl << endl;

    for (int i = 1; i <= 500; i++) {

        // هنعمل احتمالية عشوائية: 85% من الأحداث هتبقى إضافة طلب، و 15% هتبقى محاولة إلغاء
        int actionProbability = rand() % 100;

        if (actionProbability < 85) {
            // ==========================================
            //           1. Request Action
            // ==========================================

            int id = i; // الـ ID بيكبر مع اللوب عشان ميتكررش

            // توليد نوع عشوائي من 0 لـ 5 (بناءً على الـ Enum بتاعك: ODN, ODG, OT, OVN, OVC, OVG)
            ORD_TYPE type = static_cast<ORD_TYPE>(rand() % 6);

            // بيانات عشوائية منطقية
            int size = (rand() % 10) + 1;         // حجم الطلب من 1 لـ 10 أطباق
            double price = (rand() % 1000) + 100; // السعر من 100 لـ 1100 جنيه
            int tq = rand() % 50;                 // وقت الطلب العشوائي
            float distance = (rand() % 30) + 1;   // المسافة من 1 لـ 30 كيلو (للديليفري)

            /* هنا هتستخدم كلاس الـ RequestAction اللي إنت عملته.
               عدل البارامترز اللي بين الأقواس بناءً على الـ Constructor بتاعك.

               مثال لو إنت مبرمجه ياخد الداتا دي:
               RequestAction* pReq = new RequestAction(myRestaurant, id, type, size, price, tq, distance);
               pReq->Execute();
               delete pReq; // متنساش تمسح الـ Action بعد ما يتنفذ عشان الميموري!
            */

            // كود بديل ومباشر (لحد ما تظبط كلاس الـ Action لو فيه مشكلة):
            Order* newOrd = new Order(id, type, size, price, tq);
            // لو كلاس الأوردر عندك بياخد distance كمان، ضيفها.
            myRestaurant->AddOrder(newOrd);

        }
        else {
            // ==========================================
            //           2. Cancel Action
            // ==========================================

            // هنختار ID عشوائي من اللي اتعملوا قبل كده ونحاول نلغيه
            int cancelID = (rand() % i) + 1;

            /*
               هنا هتستخدم كلاس الـ CancelAction اللي إنت عملته.

               مثال:
               CancelAction* pCanc = new CancelAction(myRestaurant, cancelID);
               pCanc->Execute();
               delete pCanc;
            */

            // كود بديل ومباشر:
            myRestaurant->CancelOrder(cancelID);
        }
    }

    // 4. اختبار الطباعة (ده الإثبات إن اللوجيك بتاعك شغال 100%)
    cout << "Simulation finished successfully. Printing final state of queues:" << endl;
    cout << "---------------------------------------------------------------" << endl;

    // الدالة دي هتوريك طابور الـ OVG مترتب حسب الأولوية، والـ OVC اتمسح منه طلبات
    myRestaurant->Print_PendingOrders();

    // 5. تنظيف الذاكرة (هينادي الـ Destructor القوي اللي عملناه للمطعم)
    delete myRestaurant;

    cout << "\n===========================================" << endl;
    cout << "         End of Phase 1.2 Simulation       " << endl;
    cout << "===========================================" << endl;

    return 0;
}