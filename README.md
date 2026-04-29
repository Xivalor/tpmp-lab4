# tpmp-lab4 - Цветочная оранжерея

[![CI Pipeline](https://github.com/Xivalor/tpmp-lab4/actions/workflows/ci.yml/badge.svg)](https://github.com/Xivalor/tpmp-lab4/actions/workflows/ci.yml)

## 📋 Описание проекта

## Быстрый старт

### Требования

| Инструмент | Для чего | Установка |
|------------|----------|-----------|
| CMake (≥3.10) | Сборка проекта | `sudo apt install cmake` |
| GCC/G++ | Компилятор C++ | `sudo apt install build-essential` |
| Make | Автоматизация сборки | `sudo apt install make` |
| lcov | Покрытие кода | `sudo apt install lcov` |
| gcovr | Альтернативный отчёт | `sudo apt install gcovr` |

### Полная установка всех зависимостей (Ubuntu/Debian)

```bash
sudo apt update
sudo apt install -y cmake build-essential lcov gcovr g++

Сборка проекта
bash

git clone https://github.com/Xivalor/tpmp-lab4.git
cd tpmp-lab4
mkdir build && cd build
cmake ..
make
./tpmp-lab4

### Запуск тестов
```bash

cd build
ctest --output-on-failure --verbose

Ожидаемый результат:
text

100% tests passed, 0 tests failed out of 3

### Проверка покрытия кода
- Способ 1: Автоматический скрипт
bash

cd ~/tpmp-lab4
chmod +x check_coverage.sh
./check_coverage.sh

- Способ 2: Пошагово
bash

# 1. Сборка с флагами покрытия
rm -rf build-coverage
mkdir build-coverage && cd build-coverage
cmake .. -DCMAKE_BUILD_TYPE=Debug \
         -DENABLE_COVERAGE=ON \
         -DCMAKE_CXX_FLAGS="-coverage -fprofile-arcs -ftest-coverage" \
         -DCMAKE_EXE_LINKER_FLAGS="-coverage -fprofile-arcs -ftest-coverage"
make -j$(nproc)

# 2. Запуск тестов
ctest --output-on-failure --verbose

# 3. Генерация отчёта
lcov --capture --directory . --output-file coverage.info --ignore-errors mismatch
lcov --remove coverage.info '/usr/*' '*/tests/*' '*/googletest/*' '*/build-coverage/*' \
     --output-file coverage_filtered.info --ignore-errors unused

# 4. Просмотр процента покрытия
lcov --list coverage_filtered.info

# 5. Генерация HTML отчёта
genhtml coverage_filtered.info --output-directory coverage_html

# 6. Открыть отчёт в браузере
firefox coverage_html/index.html

📁 Структура проекта
text

tpmp-lab4/
├── src/                # Исходный код
│   ├── Flower.h/cpp    # Модуль цветов
│   ├── Composition.h/cpp # Модуль композиций
│   ├── Order.h/cpp     # Модуль заказов
│   └── main.cpp        # Точка входа
├── tests/              # Unit-тесты
│   ├── test_flowers.cpp
│   ├── test_compositions.cpp
│   └── test_orders.cpp
├── .github/workflows/  # CI/CD
│   ├── ci.yml          # Сборка и тесты
│   └── release.yml     # Релизные сборки
├── CMakeLists.txt      # Конфигурация CMake
├── Makefile            # Упрощённая сборка
└── README.md           # Документация

📝 API документация
Модуль FLOWERS (Цветы)
Функция	Описание	Пример
create_flower()	Создание цветка	Flower f(1, "Роза", "Красная", 150.0);
updatePrice()	Обновление цены (≤10%)	f.updatePrice(165.0);
updateStock()	Изменение количества	f.updateStock(100.0);
Модуль COMPOSITIONS (Композиции)
Функция	Описание	Пример
addFlower()	Добавление цветка	comp.addFlower(1, "Красная", 11);
removeFlower()	Удаление цветка	comp.removeFlower(1);
calculateCost()	Расчёт стоимости	comp.calculateCost(flowers);
Модуль ORDERS (Заказы)
Функция	Описание	Пример
create_order()	Создание заказа	Order o(1, "2026-04-20", 1, 2, "Иванов", "+7-xxx", "2026-04-20");
calculateCost()	Расчёт с наценкой	o.calculateCost(comp, flowers);
applyUrgencyFee()	Применение наценки	o.applyUrgencyFee();
⚙️ Устранение возможных ошибок
Ошибка	Решение
lcov: command not found	sudo apt install lcov
genhtml: command not found	sudo apt install lcov
mismatch warnings	Нормально, можно игнорировать
unexecuted block warnings	Нормально, можно игнорировать
