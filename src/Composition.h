#ifndef COMPOSITION_H
#define COMPOSITION_H

#include <string>
#include <vector>
#include "Flower.h"

struct FlowerInComposition {
    int flowerId;
    std::string variety;
    int quantity;
    
    FlowerInComposition(int id, const std::string& var, int qty) 
        : flowerId(id), variety(var), quantity(qty) {}
};

class Composition {
private:
    int id;
    std::string name;
    std::vector<FlowerInComposition> flowers;
    
public:
    Composition(int id, const std::string& name);
    
    // Геттеры
    int getId() const { return id; }
    std::string getName() const { return name; }
    const std::vector<FlowerInComposition>& getFlowers() const { return flowers; }
    
    // Добавление/удаление цветов
    bool addFlower(int flowerId, const std::string& variety, int quantity);
    bool removeFlower(int flowerId);
    
    // Расчёт стоимости
    double calculateCost(const std::vector<Flower>& flowersList) const;
    
    // Вспомогательные методы
    int getFlowerCount() const { return flowers.size(); }
    std::string toString() const;
    void clear();
};

#endif // COMPOSITION_H
