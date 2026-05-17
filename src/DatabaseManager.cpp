#include "DatabaseManager.h"
#include <iostream>
#include <iomanip>

DatabaseManager::DatabaseManager(const std::string& dbName) : db(nullptr) {
    if (!openDatabase(dbName)) {
        std::cerr << "Критическая ошибка: Не удалось инициализировать БД." << std::endl;
    }
}

DatabaseManager::~DatabaseManager() {
    if (db) {
        sqlite3_close(db);
    }
}

bool DatabaseManager::openDatabase(const std::string& dbName) {
    return sqlite3_open(dbName.c_str(), &db) == SQLITE_OK;
}

bool DatabaseManager::executeQuery(const std::string& sql) {
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL Error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

// Универсальный Callback для вывода табличных данных SELECT
static int tableCallback(void* NotUsed, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        std::cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << " | ";
    }
    std::cout << "\n--------------------------------------------------" << std::endl;
    return 0;
}

// 2.1 Сумма полученных денег за указанный период
void DatabaseManager::printTotalRevenue(const std::string& startDate, const std::string& endDate) {
    std::string sql = "SELECT SUM(total_cost) AS TotalRevenue FROM ORDERS WHERE order_date BETWEEN '" 
                    + startDate + "' AND '" + endDate + "';";
    std::cout << "\n=== Финансовый отчет за период с " << startDate << " по " << endDate << " ===" << std::endl;
    sqlite3_exec(db, sql.c_str(), tableCallback, 0, nullptr);
}

// 2.2 Композиция, пользующаяся максимальным спросом
void DatabaseManager::printMostPopularComposition() {
    std::string sql = "SELECT c.id, c.name, SUM(o.quantity) AS total_sold "
                      "FROM COMPOSITIONS c JOIN ORDERS o ON c.id = o.comp_id "
                      "GROUP BY c.id ORDER BY total_sold DESC LIMIT 1;";
    std::cout << "\n=== Самая популярная композиция ===" << std::endl;
    sqlite3_exec(db, sql.c_str(), tableCallback, 0, nullptr);
}

// 2.3 Количество выполненных заказов по срочности
void DatabaseManager::printOrdersCountByUrgency() {
    std::string sql = "SELECT "
                      "CASE WHEN urgency_fee > 0 THEN 'Срочный' ELSE 'Обычный' END as Срочность, "
                      "COUNT(*) as Количество "
                      "FROM ORDERS WHERE status = 'completed' GROUP BY Срочность;";
    std::cout << "\n=== Количество выполненных заказов по срочности ===" << std::endl;
    sqlite3_exec(db, sql.c_str(), tableCallback, 0, nullptr);
}

// 2.4 Количество использованных цветов по видам и сортам за период
void DatabaseManager::printUsedFlowersReport(const std::string& startDate, const std::string& endDate) {
    std::string sql = "SELECT c.name AS FlowerName, c.variety, SUM(c.quantity * o.quantity) AS TotalUsed "
                      "FROM ORDERS o JOIN COMPOSITIONS c ON o.comp_id = c.id "
                      "WHERE o.order_date BETWEEN '" + startDate + "' AND '" + endDate + "' "
                      "GROUP BY c.name, c.variety;";
    std::cout << "\n=== Отчет по использованию цветов за период ===" << std::endl;
    sqlite3_exec(db, sql.c_str(), tableCallback, 0, nullptr);
}

// 2.5 Количество проданных композиций и сумме денег по видам
void DatabaseManager::printCompositionsSalesAndRevenue() {
    std::string sql = "SELECT c.name, SUM(o.quantity) as SoldCount, SUM(o.total_cost) as Revenue "
                      "FROM ORDERS o JOIN COMPOSITIONS c ON o.comp_id = c.id "
                      "GROUP BY c.name;";
    std::cout << "\n=== Статистика продаж по видам композиций ===" << std::endl;
    sqlite3_exec(db, sql.c_str(), tableCallback, 0, nullptr);
}

// 4. Запрет на увеличение цены цветка, если композиция дорожает > 10%
bool DatabaseManager::updateFlowerPriceSecure(int flowerId, double newPrice) {
    sqlite3_stmt* stmt;
    std::string checkSql = "SELECT cost_per_unit FROM FLOWERS WHERE id = ?;";
    
    if (sqlite3_prepare_v2(db, checkSql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return false;
    sqlite3_bind_int(stmt, 1, flowerId);
    
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        double oldPrice = sqlite3_column_double(stmt, 0);
        sqlite3_finalize(stmt);
        
        double percentIncrease = ((newPrice - oldPrice) / oldPrice) * 100.0;
        if (percentIncrease > 10.0) {
            std::cerr << "Ошибка: Превышен лимит изменения стоимости композиции/цветка (> 10%)!" << std::endl;
            return false;
        }
    } else {
        sqlite3_finalize(stmt);
        return false;
    }

    std::string updateSql = "UPDATE FLOWERS SET cost_per_unit = " + std::to_string(newPrice) + " WHERE id = " + std::to_string(flowerId) + ";";
    return executeQuery(updateSql);
}

// 5. Внесение информации в таблицу заказов, расчёт стоимости и запись в таблицу отчетов
bool DatabaseManager::createOrderWithReport(const std::string& orderDate, int compId, int quantity,
                                            const std::string& customerName, const std::string& customerPhone,
                                            const std::string& completionDate) {
    // 1. Посчитаем базовую стоимость композиции динамически на основе цен из таблицы FLOWERS
    // Чтобы не перегружать код сложными выборками, сделаем вложенный SELECT стоимости цветов
    std::string costSql = 
        "SELECT SUM(c.quantity * f.cost_per_unit) FROM COMPOSITIONS c "
        "JOIN FLOWERS f ON c.flower_id = f.id WHERE c.id = " + std::to_string(compId) + ";";
    
    double baseCompCost = 150.0; // Значение по умолчанию, если букет не найден
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, costSql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            baseCompCost = sqlite3_column_double(stmt, 0);
        }
        sqlite3_finalize(stmt);
    }

    if (baseCompCost <= 0) {
        // Если такой композиции нет в таблице COMPOSITIONS
        baseCompCost = 200.0; 
    }

    double totalCost = baseCompCost * quantity;

    // 2. Вставляем новый заказ. Триггер TRIGGER_CALC_URGENCY_FEE из greenhouse.sql автоматический рассчитает наценку!
    std::string insertOrderSql = 
        "INSERT INTO ORDERS (order_date, comp_id, quantity, customer_name, customer_phone, completion_date, urgency_fee, total_cost, status) "
        "VALUES ('" + orderDate + "', " + std::to_string(compId) + ", " + std::to_string(quantity) + ", '" 
        + customerName + "', '" + customerPhone + "', '" + completionDate + "', 0.0, " + std::to_string(totalCost) + ", 'pending');";

    if (!executeQuery(insertOrderSql)) return false;

    // Получаем сгенерированный ID только что созданного заказа
    sqlite3_int64 orderId = sqlite3_last_insert_rowid(db);

    // 3. Исправленная вставка отчета (теперь передаем обязательное поле report_date)
    std::string insertReportSql = 
        "INSERT INTO ORDER_REPORTS (order_id, report_date, details, total_before, total_after) "
        "VALUES (" + std::to_string(orderId) + ", '" + orderDate + "', 'Заказ оформлен через КИС-приложение для ФИО: " 
        + customerName + "', " + std::to_string(totalCost) + ", " + std::to_string(totalCost) + ");";
    
    return executeQuery(insertReportSql);
}

// 6. Вывод всей информации о полученных заказах на указанную дату
void DatabaseManager::printOrdersByDate(const std::string& targetDate) {
    std::string sql = "SELECT * FROM ORDERS WHERE order_date = '" + targetDate + "';";
    std::cout << "\n=== Заказы, полученные на дату " << targetDate << " ===" << std::endl;
    sqlite3_exec(db, sql.c_str(), tableCallback, 0, nullptr);
}