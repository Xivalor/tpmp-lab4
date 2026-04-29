#include <gtest/gtest.h>
#include "Composition.h"
#include "Flower.h"

TEST(CompositionTest, CreateComposition) {
    Composition c(1, "Букет невесты");
    EXPECT_EQ(c.getId(), 1);
    EXPECT_EQ(c.getName(), "Букет невесты");
    EXPECT_EQ(c.getFlowerCount(), 0);
}

TEST(CompositionTest, AddFlower) {
    Composition c(1, "Букет невесты");
    EXPECT_TRUE(c.addFlower(1, "Красная", 11));
    EXPECT_EQ(c.getFlowerCount(), 1);
}

TEST(CompositionTest, AddDuplicateFlowerFails) {
    Composition c(1, "Букет невесты");
    c.addFlower(1, "Красная", 11);
    EXPECT_FALSE(c.addFlower(1, "Красная", 5));
}

TEST(CompositionTest, RemoveFlower) {
    Composition c(1, "Букет невесты");
    c.addFlower(1, "Красная", 11);
    c.addFlower(2, "Белая", 7);
    EXPECT_EQ(c.getFlowerCount(), 2);
    EXPECT_TRUE(c.removeFlower(1));
    EXPECT_EQ(c.getFlowerCount(), 1);
}

TEST(CompositionTest, RemoveNonexistentFlowerFails) {
    Composition c(1, "Букет невесты");
    c.addFlower(1, "Красная", 11);
    EXPECT_FALSE(c.removeFlower(99));
}

TEST(CompositionTest, CalculateCost) {
    std::vector<Flower> flowers;
    flowers.emplace_back(1, "Роза", "Красная", 150.0);
    flowers.emplace_back(2, "Роза", "Белая", 160.0);
    
    Composition c(1, "Букет невесты");
    c.addFlower(1, "Красная", 11);
    c.addFlower(2, "Белая", 11);
    
    double cost = c.calculateCost(flowers);
    EXPECT_DOUBLE_EQ(cost, 3410.0);
}

TEST(CompositionTest, ClearComposition) {
    Composition c(1, "Букет невесты");
    c.addFlower(1, "Красная", 11);
    c.addFlower(2, "Белая", 7);
    EXPECT_EQ(c.getFlowerCount(), 2);
    c.clear();
    EXPECT_EQ(c.getFlowerCount(), 0);
}
