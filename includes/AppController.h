#ifndef APP_CONTROLLER_H
#define APP_CONTROLLER_H

#include "DatabaseManager.h"
#include "AuthSystem.h"

class AppController {
private:
    DatabaseManager dbManager;
    UserRole currentRole;
    std::string currentCustomerName;

    void runAdminMenu();
    void runCustomerMenu();

public:
    AppController(const std::string& dbName);
    void start();
};

#endif // APP_CONTROLLER_H