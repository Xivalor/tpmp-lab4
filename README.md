# tpmp-lab4 - Цветочная оранжерея

[Отчет](https://docs.google.com/document/d/1pIygK0BVce_K1gdQNub3DO0s0ci9rS2q7zytiDCnjts/edit?hl=ru&tab=t.0)

[![CI Pipeline](https://github.com/Xivalor/tpmp-lab4/actions/workflows/ci.yml/badge.svg)](https://github.com/Xivalor/tpmp-lab4/actions/workflows/ci.yml)  
[![Code Coverage](https://github.com/Xivalor/tpmp-lab4/actions/workflows/coverage.yml/badge.svg)](https://github.com/Xivalor/tpmp-lab4/actions/workflows/coverage.yml)

## 📋 Описание проекта

Проект **Цветочная оранжерея** на C++. КИС (Корпоративная информационная система) для управления заказами цветочных композиций с персистентным хранением данных в СУБД SQLite3.

В системе реализованы:
- **Ролевая модель доступа**: Дирекция (`admin`) и Покупатели (`customer`).
- **Автоматический расчёт наценки за срочность**:
  - **25%** — выполнение заказа в течение суток (≤1 день)
  - **15%** — выполнение заказа в течение двух суток (2 дня)
- **База данных**: Хранение истории заказов, каталога цветов, логов активности пользователей и генерация отчетов.

---

## Быстрый старт

### Требования

| Инструмент | Для чего | Установка (Ubuntu) |
|------------|----------|--------------------|
| CMake (≥3.10) | Сборка проекта | `sudo apt install cmake` |
| GCC/G++ | Компилятор C++ (стандарт C++17) | `sudo apt install build-essential` |
| SQLite3 (dev) | Библиотека СУБД | `sudo apt install libsqlite3-dev sqlite3` |
| lcov | Покрытие кода | `sudo apt install lcov` |
| gcovr | Альтернативный отчёт покрытия | `sudo apt install gcovr` |

### Полная установка всех зависимостей (Ubuntu/Debian)
```bash
sudo apt update
sudo apt install -y cmake build-essential libsqlite3-dev sqlite3 lcov gcovr g++
```

### Сборка проекта
```bash
git clone [https://github.com/Xivalor/tpmp-lab4.git](https://github.com/Xivalor/tpmp-lab4.git)
cd tpmp-lab4

mkdir build && cd build
cmake ..
make

../bin/greenhouse_app
```
> Важно для авторизации: При запуске используйте дефолтные данные из дампа БД.
> Например, Логин: customer1 | Пароль: hash_customer123 или Логин: admin | Пароль: hash_admin123.

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
├── bin/
├── includes/           # Заголовочные файлы (.h)
│   ├── Flower.h
│   ├── Composition.h
│   ├── Order.h
│   ├── DatabaseManager.h
│   ├── AuthSystem.h
│   └── AppController.h
├── src/                # Исходный код (.cpp)
│   ├── Flower.cpp
│   ├── Composition.cpp
│   ├── Order.cpp
│   ├── DatabaseManager.cpp
│   ├── AppController.cpp
│   └── main.cpp        # Инициализация AppController
├── tests/
│   ├── CMakeLists.txt
│   └── unit_tests.cpp
├── .github/workflows/  # Автоматизация CI/CD
│   ├── ci.yml
│   ├── coverage.yml
│   └── release.yml
├── CMakeLists.txt
└── README.md           # Документация проекта
```
## API Documentation

### AuthSystem Class

| Method | Parameters | Return | Description |
|--------|------------|--------|-------------|
| `authenticate()` | `username: string, password: string` | `UserRole` | Static method. Validates credentials and returns role (`ADMIN`, `CUSTOMER`, `NONE`) |

### DatabaseManager Class

| Method | Parameters | Return | Description |
|--------|------------|--------|-------------|
| `DatabaseManager()` | `dbName: string` | - | Constructor. Initializes database connection |
| `openDatabase()` | `dbName: string` | `bool` | Opens connection to SQLite3 database |
| `executeQuery()` | `sql: string` | `bool` | Executes raw SQL query (INSERT/UPDATE/DELETE) |
| `createOrderWithReport()` | `orderDate: string, compId: int, quantity: int, customerName: string, customerPhone: string, completionDate: string` | `bool` | Business transaction. Inserts order into database and writes metadata to `ORDER_REPORTS` |
| `printOrdersByDate()` | `targetDate: string` | `void` | Selects and prints formatted table of orders for specific date |

### AppController Class

| Method | Parameters | Return | Description |
|--------|------------|--------|-------------|
| `AppController()` | `dbName: string` | - | Constructor. Links application controller with a database |
| `start()` | - | `void` | Runs main runtime loop (requests login/password and routes user to appropriate menu) |

### Flower Class

| Method | Parameters | Return | Description |
|--------|------------|--------|-------------|
| `Flower()` | `id: int, name: string, variety: string, cost: double` | - | Constructor. Throws `std::invalid_argument` if cost $\le$ 0 |
| `updatePrice()` | `newPrice: double` | `bool` | Updates unit cost. Returns false if price increase >10% |
| `updateStock()` | `amount: double` | `bool` | Modifies warehouse stock. Prevents negative stock balance |
| `toString()` | - | `string` | Returns formatted string with flower details |

### Composition Class

| Method | Parameters | Return | Description |
|--------|------------|--------|-------------|
| `Composition()` | `id: int, name: string` | - | Constructor. Initializes an empty flower recipe |
| `addFlower()` | `flowerId: int, variety: string, quantity: int` | `bool` | Adds flower to composition. Prevents duplicates and $\le$ 0 quantities |
| `removeFlower()` | `flowerId: int` | `bool` | Removes flower from composition recipe by ID |
| `calculateCost()` | `flowersList: const vector<Flower>&` | `double` | Calculates pure cost based on matching IDs from flowers inventory |
| `toString()` | - | `string` | Returns formatted text layout of the composition recipe |

### Order Class

| Method | Parameters | Return | Description |
|--------|------------|--------|-------------|
| `Order()` | `id: int, orderDate: string, compositionId: int, quantity: int, customerName: string, customerPhone: string, completionDate: string` | - | Constructor. Validates date bounds and item count |
| `calculateCost()` | `composition: const Composition&, flowersList: const vector<Flower>&` | `double` | Calculates total cost multiplied by quantity including urgency fee |
| `applyUrgencyFee()` | - | `double` | Applies fee based on days difference between order and completion |
| `daysBetween()` | `date1: string, date2: string` | `int` | Static helper. Parses `YYYY-MM-DD` and returns days delta |
| `statusToString()` | `status: OrderStatus` | `string` | Static helper. Converts internal status enum to string literal |
| `toString()` | - | `string` | Returns formatted summary invoice text |

### Urgency Fee Rules

| Completion Time (Days Delta) | Fee |
|------------------------------|-----|
| Same day / Next day ($\le$ 1 day) | 25% |
| 2 days | 15% |
| 3+ days | 0% |

## Troubleshooting & Common Errors

| Error / Warning | Cause | Solution |
|-----------------|-------|----------|
| `Ошибка аутентификации! Доступ закрыт.` | Application launched from a directory without a valid `greenhouse` database binary. SQLite auto-created an empty file. | Run application from build directory where `greenhouse` file resides or manually copy it to the current path. |
| `SQL Error: no such table: USERS` | The database file was created completely blank. Сhema was never initialized. | Initialize schema dump before running: `sqlite3 greenhouse < ../greenhouse.sql` |
| `CMake Error: SQLite3 REQUIRED not found` | SQLite3 development headers are missing on the host OS. | Install development headers: `sudo apt install libsqlite3-dev` |
| `lcov: command not found` | lcov utility not installed | `sudo apt install lcov` |
| `genhtml: command not found` | lcov toolchain missing | `sudo apt install lcov` |
| `mismatched end line` | Minor version mismatch between gcc and lcov | **Safe to ignore** |
| `unexecuted block` | Compiler optimization side effect | **Safe to ignore** |
| `gcov: version mismatch` | System uses different standalone gcc version | `sudo apt install gcc-13 g++-13` |
| `CMake Error: No known features for CXX` | Missing host C++ compiler toolchain | `sudo apt install g++` |
| `lcov: ERROR: no .gcda files found in .` | `ctest` was not executed or coverage flags were omitted during compilation. | Build project with `-DENABLE_COVERAGE=ON` and make sure to run tests before code analysis. |
