#include <gtest/gtest.h>
#include "Flower.h"

TEST(FlowerTest, CreateValidFlower) {
    Flower f(1, "Роза", "Красная", 150.0);
    EXPECT_EQ(f.getId(), 1);
    EXPECT_EQ(f.getName(), "Роза");
    EXPECT_EQ(f.getCostPerUnit(), 150.0);
}

TEST(FlowerTest, CreateInvalidFlowerThrows) {
    EXPECT_THROW(Flower(1, "Роза", "Красная", -150.0), std::invalid_argument);
}

TEST(FlowerTest, UpdatePriceValid) {
    Flower f(1, "Роза", "Красная", 150.0);
    bool result = f.updatePrice(165.0);
    EXPECT_TRUE(result);
    EXPECT_EQ(f.getCostPerUnit(), 165.0);
}

TEST(FlowerTest, UpdatePriceInvalid) {
    Flower f(1, "Роза", "Красная", 150.0);
    bool result = f.updatePrice(170.0);
    EXPECT_FALSE(result);
    EXPECT_EQ(f.getCostPerUnit(), 150.0);
}

TEST(FlowerTest, UpdateStockValid) {
    Flower f(1, "Роза", "Красная", 150.0);
    EXPECT_TRUE(f.updateStock(100.0));
    EXPECT_EQ(f.getStockAmount(), 100.0);
}

TEST(FlowerTest, UpdateStockInsufficient) {
    Flower f(1, "Роза", "Красная", 150.0);
    f.updateStock(100.0);
    EXPECT_FALSE(f.updateStock(-150.0));
    EXPECT_EQ(f.getStockAmount(), 100.0);
}

TEST(FlowerTest, CanIncreasePrice) {
    Flower f(1, "Роза", "Красная", 150.0);
    EXPECT_TRUE(f.canIncreasePrice(10.0));
    EXPECT_FALSE(f.canIncreasePrice(15.0));
}

TEST(FlowerTest, ToString) {
    Flower f(1, "Роза", "Красная", 150.0);
    std::string str = f.toString();
    EXPECT_TRUE(str.find("Роза") != std::string::npos);
}
