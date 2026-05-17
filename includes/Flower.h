#ifndef FLOWER_H
#define FLOWER_H

#include <string>
#include <iostream>

class Flower {
private:
    int id;
    std::string name;
    std::string variety;
    double costPerUnit;
    double stockAmount;
    
public:
    Flower(int id, const std::string& name, const std::string& variety, double cost);
    
    // Геттеры
    int getId() const { return id; }
    std::string getName() const { return name; }
    std::string getVariety() const { return variety; }
    double getCostPerUnit() const { return costPerUnit; }
    double getStockAmount() const { return stockAmount; }
    
    // Сеттеры с проверками
    bool updatePrice(double newPrice);
    bool updateStock(double amount);
    
    // Вспомогательные методы
    std::string toString() const;
    bool canIncreasePrice(double percentage) const;
    
    // Статическая константа
    static constexpr double MAX_PRICE_INCREASE_PERCENT = 10.0;
};

#endif // FLOWER_H
