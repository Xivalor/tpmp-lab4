# tpmp-lab4 - Цветочная оранжерея

[Отчет](https://docs.google.com/document/d/1pIygK0BVce_K1gdQNub3DO0s0ci9rS2q7zytiDCnjts/edit?hl=ru&tab=t.0)

[![CI Pipeline](https://github.com/Xivalor/tpmp-lab4/actions/workflows/ci.yml/badge.svg)](https://github.com/Xivalor/tpmp-lab4/actions/workflows/ci.yml)  
[![Code Coverage](https://github.com/Xivalor/tpmp-lab4/actions/workflows/coverage.yml/badge.svg)](https://github.com/Xivalor/tpmp-lab4/actions/workflows/coverage.yml)

## 📋 Описание проекта

Проект **Цветочная оранжерея** на C++. Система для управления заказами цветочных композиций с автоматическим расчётом наценки за срочность:
- **25%** — выполнение заказа в течение суток
- **15%** — выполнение заказа в течение двух суток
  
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
```

### Сборка проекта
```bash
git clone https://github.com/Xivalor/tpmp-lab4.git
cd tpmp-lab4
mkdir build && cd build
cmake ..
make
./tpmp-lab4
```

### Запуск тестов
```bash
cd build
ctest --output-on-failure --verbose
```

Ожидаемый результат:
```
100% tests passed, 0 tests failed out of 3
```

### Проверка покрытия кода
- Способ 1: Автоматический скрипт
```bash

cd ~/tpmp-lab4
chmod +x check_coverage.sh
./check_coverage.sh
```

- Способ 2: Пошагово
```bash

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
```

Структура проекта
```
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
```
## API Documentation

### Flower Class

| Method | Parameters | Return | Description |
|--------|------------|--------|-------------|
| `Flower()` | `id: int, name: string, variety: string, cost: double` | - | Constructor |
| `updatePrice()` | `newPrice: double` | `bool` | Updates price, returns false if >10% increase |
| `updateStock()` | `amount: double` | `bool` | Updates stock amount |
| `toString()` | - | `string` | Returns formatted string |

### Composition Class

| Method | Parameters | Return | Description |
|--------|------------|--------|-------------|
| `addFlower()` | `flowerId: int, variety: string, quantity: int` | `bool` | Adds flower to composition |
| `removeFlower()` | `flowerId: int` | `bool` | Removes flower from composition |
| `calculateCost()` | `flowers: vector<Flower>&` | `double` | Calculates total cost |

### Order Class

| Method | Parameters | Return | Description |
|--------|------------|--------|-------------|
| `create_order()` | `id, orderDate, compId, quantity, customerName, phone, completionDate` | `Order*` | Creates new order |
| `calculateCost()` | `comp: Composition&, flowers: vector<Flower>&` | `double` | Calculates cost with urgency fee |
| `applyUrgencyFee()` | - | `double` | Applies 25% (1 day) or 15% (2 days) fee |

### Urgency Fee Rules

| Completion Time | Fee |
|----------------|-----|
| Same day (≤1 day) | 25% |
| Next day (2 days) | 15% |
| 3+ days | 0% |

## Troubleshooting & Common Errors

| Error / Warning | Cause | Solution |
|-----------------|-------|----------|
| `lcov: command not found` | lcov not installed | `sudo apt install lcov` |
| `genhtml: command not found` | lcov missing | `sudo apt install lcov` |
| `mismatched end line` | Version mismatch between gcc and lcov | **Safe to ignore** |
| `unexecuted block` | Optimization flags | **Safe to ignore** |
| `gcov: version mismatch` | Different gcc version | `sudo apt install gcc-13 g++-13` |
| `CMake Error: No known features for CXX` | Missing C++ compiler | `sudo apt install g++` |
| `LNK2038: mismatch detected for 'RuntimeLibrary'` | Windows static/dynamic mismatch | Use Ubuntu for coverage |
