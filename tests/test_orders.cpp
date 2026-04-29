#include <gtest/gtest.h>
#include "Order.h"
#include "Composition.h"
#include "Flower.h"

TEST(OrderTest, CreateValidOrder) {
    Order o(1, "2026-04-01", 1, 2, "Иванов", "+7-912-345-67-89", "2026-04-05");
    EXPECT_EQ(o.getId(), 1);
    EXPECT_EQ(o.getCustomerName(), "Иванов");
}

TEST(OrderTest, CreateOrderWithInvalidQuantityThrows) {
    EXPECT_THROW(Order(1, "2026-04-01", 1, -2, "Иванов", "+7-912-345-67-89", "2026-04-05"),
                 std::invalid_argument);
}

TEST(OrderTest, CreateOrderWithEmptyNameThrows) {
    EXPECT_THROW(Order(1, "2026-04-01", 1, 2, "", "+7-912-345-67-89", "2026-04-05"),
                 std::invalid_argument);
}

TEST(OrderTest, UrgencyFeeOneDay) {
    Order o(1, "2026-04-01", 1, 2, "Иванов", "+7-912-345-67-89", "2026-04-01");
    double fee = o.applyUrgencyFee();
    EXPECT_EQ(fee, 0.0); // totalCost ещё не установлен
}

TEST(OrderTest, DaysBetween) {
    EXPECT_EQ(Order::daysBetween("2026-04-01", "2026-04-01"), 0);
    EXPECT_EQ(Order::daysBetween("2026-04-01", "2026-04-02"), 1);
    EXPECT_EQ(Order::daysBetween("2026-04-01", "2026-04-03"), 2);
}

TEST(OrderTest, UpdateStatus) {
    Order o(1, "2026-04-01", 1, 2, "Иванов", "+7-912-345-67-89", "2026-04-05");
    o.updateStatus(OrderStatus::COMPLETED);
    EXPECT_EQ(o.getStatus(), OrderStatus::COMPLETED);
}

TEST(OrderTest, StatusToString) {
    EXPECT_EQ(Order::statusToString(OrderStatus::PENDING), "pending");
    EXPECT_EQ(Order::statusToString(OrderStatus::COMPLETED), "completed");
    EXPECT_EQ(Order::statusToString(OrderStatus::CANCELLED), "cancelled");
}

TEST(OrderTest, FullCostCalculation) {
    std::vector<Flower> flowers;
    flowers.emplace_back(1, "Роза", "Красная", 150.0);
    flowers.emplace_back(2, "Роза", "Белая", 160.0);
    
    Composition c(1, "Букет невесты");
    c.addFlower(1, "Красная", 11);
    c.addFlower(2, "Белая", 11);
    
    Order o(1, "2026-04-01", 1, 2, "Иванов", "+7-912-345-67-89", "2026-04-01");
    double cost = o.calculateCost(c, flowers);
    EXPECT_DOUBLE_EQ(cost, 8525.0);
}
