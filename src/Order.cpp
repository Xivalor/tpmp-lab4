#include "Order.h"
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cmath>

int Order::daysBetween(const std::string& date1, const std::string& date2) {
    // Простое приближение для учебных целей
    int y1, m1, d1, y2, m2, d2;
    sscanf(date1.c_str(), "%d-%d-%d", &y1, &m1, &d1);
    sscanf(date2.c_str(), "%d-%d-%d", &y2, &m2, &d2);
    
    int days1 = y1 * 365 + m1 * 30 + d1;
    int days2 = y2 * 365 + m2 * 30 + d2;
    return days2 - days1;
}

Order::Order(int id, const std::string& orderDate, int compositionId, int quantity,
             const std::string& customerName, const std::string& customerPhone,
             const std::string& completionDate)
    : id(id), orderDate(orderDate), compositionId(compositionId), quantity(quantity),
      customerName(customerName), customerPhone(customerPhone), completionDate(completionDate),
      urgencyFee(0), totalCost(0), status(OrderStatus::PENDING) {
    
    if (quantity <= 0) {
        throw std::invalid_argument("Количество должно быть положительным");
    }
    if (customerName.empty()) {
        throw std::invalid_argument("Имя покупателя не может быть пустым");
    }
    if (daysBetween(orderDate, completionDate) < 0) {
        throw std::invalid_argument("Дата выполнения не может быть раньше даты заказа");
    }
}

double Order::calculateCost(const Composition& composition, const std::vector<Flower>& flowersList) {
    double compCost = composition.calculateCost(flowersList);
    totalCost = compCost * quantity;
    applyUrgencyFee();
    return totalCost;
}

double Order::applyUrgencyFee() {
    int days = daysBetween(orderDate, completionDate);
    
    if (days <= 1) {
        urgencyFee = totalCost * 0.25;  // 25% за 1 день
    } else if (days == 2) {
        urgencyFee = totalCost * 0.15;  // 15% за 2 дня
    } else {
        urgencyFee = 0.0;
    }
    
    totalCost += urgencyFee;
    return urgencyFee;
}

void Order::updateStatus(OrderStatus newStatus) {
    status = newStatus;
}

std::string Order::statusToString(OrderStatus status) {
    switch (status) {
        case OrderStatus::PENDING:   return "pending";
        case OrderStatus::COMPLETED: return "completed";
        case OrderStatus::CANCELLED: return "cancelled";
        default:                     return "unknown";
    }
}

std::string Order::toString() const {
    std::ostringstream oss;
    oss << "Заказ #" << id << ": " << customerName 
        << ", сумма: " << std::fixed << std::setprecision(2) << totalCost 
        << " руб. (наценка: " << urgencyFee << " руб.), статус: " << statusToString(status);
    return oss.str();
}
