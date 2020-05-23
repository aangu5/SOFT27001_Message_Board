#include <iostream>
#include "Dashboard.h"
#include "Database.h"
using namespace std;

//main function instantiating a dashboard object, opening the dashboard and running the mainMenu() function
int main()
{
    Dashboard startup;
    if (startup.openDashboard()) {
        startup.mainMenu();
    }
    else {
        cout << "There has been a fault with the database, please contact support." << endl;
    }
}
