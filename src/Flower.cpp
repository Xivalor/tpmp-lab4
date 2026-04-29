#include "Flower.h"
#include <sstream>
#include <iomanip>

Flower::Flower(int id, const std::string& name, const std::string& variety, double cost) 
    : id(id), name(name), variety(variety), costPerUnit(cost), stockAmount(0) {
    if (cost <= 0) {
        throw std::invalid_argument("Стоимость цветка должна быть положительной");
    }
}

bool Flower::updatePrice(double newPrice) {
    if (newPrice <= 0) {
        std::cerr << "Ошибка: новая цена должна быть положительной" << std::endl;
        return false;
    }
    
    double increasePercent = (newPrice - costPerUnit) / costPerUnit * 100;
    
    if (increasePercent > MAX_PRICE_INCREASE_PERCENT) {
        std::cerr << "Ошибка: нельзя увеличить цену более чем на " 
                  << MAX_PRICE_INCREASE_PERCENT << "%" << std::endl;
        return false;
    }
    
    costPerUnit = newPrice;
    return true;
}

bool Flower::updateStock(double amount) {
    if (amount < 0 && stockAmount + amount < 0) {
        std::cerr << "Ошибка: недостаточно цветов на складе" << std::endl;
        return false;
    }
    stockAmount += amount;
    return true;
}

std::string Flower::toString() const {
    std::ostringstream oss;
    oss << "Цветок #" << id << ": " << name << " (" << variety 
        << "), цена: " << std::fixed << std::setprecision(2) << costPerUnit 
        << " руб., в наличии: " << stockAmount << " шт.";
    return oss.str();
}

bool Flower::canIncreasePrice(double percentage) const {
    return percentage <= MAX_PRICE_INCREASE_PERCENT;
}
