#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <vector>
#include "Flower.h"
#include "Composition.h"

enum class OrderStatus {
    PENDING,
    COMPLETED,
    CANCELLED
};

class Order {
private:
    int id;
    std::string orderDate;
    int compositionId;
    int quantity;
    std::string customerName;
    std::string customerPhone;
    std::string completionDate;
    double urgencyFee;
    double totalCost;
    OrderStatus status;
    
public:
    Order(int id, const std::string& orderDate, int compositionId, int quantity,
          const std::string& customerName, const std::string& customerPhone,
          const std::string& completionDate);
    
    // Геттеры
    int getId() const { return id; }
    std::string getOrderDate() const { return orderDate; }
    int getCompositionId() const { return compositionId; }
    int getQuantity() const { return quantity; }
    std::string getCustomerName() const { return customerName; }
    std::string getCustomerPhone() const { return customerPhone; }
    std::string getCompletionDate() const { return completionDate; }
    double getUrgencyFee() const { return urgencyFee; }
    double getTotalCost() const { return totalCost; }
    OrderStatus getStatus() const { return status; }
    
    // Методы
    double calculateCost(const Composition& composition, const std::vector<Flower>& flowersList);
    double applyUrgencyFee();
    void updateStatus(OrderStatus newStatus);
    std::string toString() const;
    
    // Вспомогательные методы
    static int daysBetween(const std::string& date1, const std::string& date2);
    static std::string statusToString(OrderStatus status);
};

#endif // ORDER_H
