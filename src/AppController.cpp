#include "AppController.h"
#include <iostream>

// Вспомогательная структура для передачи данных из callback авторизации
struct AuthData {
    UserRole role = UserRole::NONE;
    std::string fullName = "";
    bool found = false;
};

// Callback для проверки логина и пароля в БД
static int authCallback(void* data, int argc, char** argv, char** azColName) {
    AuthData* auth = static_cast<AuthData*>(data);
    std::string roleStr = "";
    
    for (int i = 0; i < argc; i++) {
        if (std::string(azColName[i]) == "role" && argv[i]) {
            roleStr = argv[i];
        }
        if (std::string(azColName[i]) == "full_name" && argv[i]) {
            auth->fullName = argv[i];
        }
    }
    
    auth->role = AuthSystem::stringToRole(roleStr);
    auth->found = true;
    return 0;
}

AppController::AppController(const std::string& dbName) 
    : dbManager(dbName), currentRole(UserRole::NONE) {}

void AppController::start() {
    std::cout << "========================================\n";
    std::cout << "   КИС: Цветочная оранжерея приветствует вас\n";
    std::cout << "========================================\n";

    std::string username, password;
    std::cout << "Введите логин (например, customer1 или admin): ";
    std::cin >> username;
    std::cout << "Введите пароль (например, hash_customer123): ";
    std::cin >> password;

    // Ищем пользователя в реальной таблице USERS
    std::string sql = "SELECT role, full_name FROM USERS WHERE username = '" 
                      + username + "' AND password_hash = '" + password + "' LIMIT 1;";
    
    AuthData authResult;
    char* errMsg = nullptr;
    
    // Вызываем sqlite3_exec через dbManager.getDB() или напрямую
    int rc = sqlite3_exec(dbManager.getDB(), sql.c_str(), authCallback, &authResult, &errMsg);
    
    if (rc != SQLITE_OK || !authResult.found) {
        std::cout << "Ошибка аутентификации! Неверный логин или пароль.\n";
        if (errMsg) sqlite3_free(errMsg);
        return;
    }

    currentRole = authResult.role;
    // Очень важно: сохраняем ФИО (например, "Сидоров Алексей Викторович"), 
    // потому что именно оно связано с заказами в таблице ORDERS!
    currentCustomerName = authResult.fullName; 

    std::cout << "\nУспешный вход! Добро пожаловать, " << currentCustomerName << ".\n";

    if (currentRole == UserRole::ADMIN || currentRole == UserRole::MANAGER) {
        runAdminMenu();
    } else if (currentRole == UserRole::CUSTOMER) {
        runCustomerMenu();
    }
}

void AppController::runAdminMenu() {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n=== МЕНЮ АДМИНИСТРАТОРА / МЕНЕДЖЕРА ===\n"
                  << "1. Финансовый отчет по заказам\n"
                  << "2. Самая популярная композиция\n"
                  << "3. Статистика по срочности\n"
                  << "4. Анализ расхода цветов\n"
                  << "5. Статистика продаж и выручки композиций\n"
                  << "6. Изменить цену на цветок (Защита 10%)\n"
                  << "7. Посмотреть заказы на дату\n"
                  << "0. Выход\n"
                  << "Выберите действие: ";
        std::cin >> choice;

        switch (choice) {
            case 1: dbManager.printTotalRevenue("2026-04-01", "2026-04-30"); break;
            case 2: dbManager.printMostPopularComposition(); break;
            case 3: dbManager.printOrdersCountByUrgency(); break;
            case 4: dbManager.printUsedFlowersReport("2026-04-01", "2026-04-30"); break;
            case 5: dbManager.printCompositionsSalesAndRevenue(); break;
            case 6: {
                int id; double price;
                std::cout << "ID цветка: "; std::cin >> id;
                std::cout << "Новая цена: "; std::cin >> price;
                dbManager.updateFlowerPriceSecure(id, price);
                break;
            }
            case 7: {
                std::string date;
                std::cout << "Введите дату (ГГГГ-ММ-ДД): "; std::cin >> date;
                dbManager.printOrdersByDate(date);
                break;
            }
        }
    }
}

void AppController::runCustomerMenu() {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n=== МЕНЮ ПОКУПАТЕЛЯ ===\n"
                  << "1. Оформить новый заказ\n"
                  << "2. Посмотреть историю моих заказов\n"
                  << "0. Выход\n"
                  << "Выберите действие: ";
        std::cin >> choice;

        if (choice == 1) {
            std::string oDate, compId, qty, phone, cDate;
            std::cout << "Дата заказа (ГГГГ-ММ-ДД): "; std::cin >> oDate;
            std::cout << "ID Композиции (1-18): "; std::cin >> compId;
            std::cout << "Количество букетов: "; std::cin >> qty;
            std::cout << "Ваш телефон: "; std::cin >> phone;
            std::cout << "Дата исполнения (ГГГГ-ММ-ДД): "; std::cin >> cDate;

            if (dbManager.createOrderWithReport(oDate, std::stoi(compId), std::stoi(qty), currentCustomerName, phone, cDate)) {
                std::cout << "Заказ успешно создан и занесен в базу данных!\n";
            }
        } else if (choice == 2) {
            // Ищем заказы строго по ФИО текущего авторизованного покупателя
            std::cout << "\n--- История заказов для: " << currentCustomerName << " ---\n";
            std::string sql = "SELECT id, order_date, comp_id, quantity, total_cost, status FROM ORDERS WHERE customer_name = '" + currentCustomerName + "';";
            
            char* errMsg = nullptr;
            // Используем лямбду или функцию для красивого вывода таблиц
            sqlite3_exec(dbManager.getDB(), sql.c_str(), [](void*, int argc, char** argv, char** azColName) -> int {
                for (int i = 0; i < argc; i++) {
                    std::cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << " | ";
                }
                std::cout << "\n--------------------------------------------------\n";
                return 0;
            }, nullptr, &errMsg);
            if (errMsg) sqlite3_free(errMsg);
        }
    }
}