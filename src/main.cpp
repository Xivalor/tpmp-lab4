#include <iostream>
#include <vector>
#include <memory>
#include "Flower.h"
#include "Composition.h"
#include "Order.h"

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "   Цветочная оранжерея - C++ version" << std::endl;
    std::cout << "========================================" << std::endl << std::endl;
    
    try {
        // Создаём цветы
        std::vector<Flower> flowers;
        flowers.emplace_back(1, "Роза", "Красная", 150.0);
        flowers.emplace_back(2, "Тюльпан", "Жёлтый", 80.0);
        flowers.emplace_back(3, "Лилия", "Белая", 200.0);
        
        // Создаём композицию
        Composition composition(1, "Букет невесты");
        composition.addFlower(1, "Красная", 11);
        composition.addFlower(3, "Белая", 7);
        
        // Создаём заказ (срочный - выполнение в тот же день)
        Order order(1, "2026-04-20", 1, 2, "Иванов Иван", 
                    "+7-912-345-67-89", "2026-04-20");
        
        // Рассчитываем стоимость
        order.calculateCost(composition, flowers);
        
        // Выводим информацию
        std::cout << "=== Цветы ===" << std::endl;
        for (const auto& flower : flowers) {
            std::cout << flower.toString() << std::endl;
        }
        
        std::cout << std::endl << "=== Композиция ===" << std::endl;
        std::cout << composition.toString() << std::endl;
        
        std::cout << std::endl << "=== Заказ ===" << std::endl;
        std::cout << order.toString() << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }
    
    std::cout << std::endl << "До свидания! Meow!" << std::endl;
    return 0;
}
