#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <string>
#include <vector>
#include <sqlite3.h>

class DatabaseManager {
private:
    sqlite3* db;
    bool openDatabase(const std::string& dbName);

public:
    DatabaseManager(const std::string& dbName);
    ~DatabaseManager();

    sqlite3* getDB() const { return db; }

    // Пункт 2: Выборки (SELECT)
    void printTotalRevenue(const std::string& startDate, const std::string& endDate);
    void printMostPopularComposition();
    void printOrdersCountByUrgency();
    void printUsedFlowersReport(const std::string& startDate, const std::string& endDate);
    void printCompositionsSalesAndRevenue();

    // Пункт 3: C.R.U.D. Операции
    bool executeQuery(const std::string& sql);

    // Пункт 4: Бизнес-логика контроля цен (10%)
    bool updateFlowerPriceSecure(int flowerId, double newPrice);

    // Пункт 5: Оформление заказа с расчётом стоимости
    bool createOrderWithReport(const std::string& orderDate, int compId, int quantity,
                               const std::string& customerName, const std::string& customerPhone,
                               const std::string& completionDate);

    // Пункт 6: Вывод заказов на дату
    void printOrdersByDate(const std::string& targetDate);
};

#endif // DATABASE_MANAGER_H