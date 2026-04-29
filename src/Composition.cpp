#include "Composition.h"
#include <sstream>
#include <algorithm>

Composition::Composition(int id, const std::string& name) 
    : id(id), name(name) {}

bool Composition::addFlower(int flowerId, const std::string& variety, int quantity) {
    if (quantity <= 0) {
        std::cerr << "Ошибка: количество цветов должно быть положительным" << std::endl;
        return false;
    }
    
    // Проверка на дубликат
    for (const auto& flower : flowers) {
        if (flower.flowerId == flowerId) {
            std::cerr << "Ошибка: цветок уже добавлен в композицию" << std::endl;
            return false;
        }
    }
    
    flowers.emplace_back(flowerId, variety, quantity);
    return true;
}

bool Composition::removeFlower(int flowerId) {
    auto it = std::find_if(flowers.begin(), flowers.end(),
        [flowerId](const FlowerInComposition& f) { return f.flowerId == flowerId; });
    
    if (it != flowers.end()) {
        flowers.erase(it);
        return true;
    }
    
    std::cerr << "Ошибка: цветок с id " << flowerId << " не найден в композиции" << std::endl;
    return false;
}

double Composition::calculateCost(const std::vector<Flower>& flowersList) const {
    double totalCost = 0.0;
    
    for (const auto& flowerInComp : flowers) {
        for (const auto& flower : flowersList) {
            if (flower.getId() == flowerInComp.flowerId) {
                totalCost += flower.getCostPerUnit() * flowerInComp.quantity;
                break;
            }
        }
    }
    
    return totalCost;
}

std::string Composition::toString() const {
    std::ostringstream oss;
    oss << "Композиция #" << id << ": " << name 
        << " (состоит из " << flowers.size() << " цветов)";
    return oss.str();
}

void Composition::clear() {
    flowers.clear();
}
