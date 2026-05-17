#include <iostream>
#include <vector>
#include <memory>
#include "Flower.h"
#include "Composition.h"
#include "Order.h"
#include "AppController.h"

int main() {
    // Указываем имя вашего файла с тестовыми данными
    AppController app("greenhouse"); 
    app.start();
    return 0;
}