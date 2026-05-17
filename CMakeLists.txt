cmake_minimum_required(VERSION 3.10)
project(greenhouse_project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ENABLE_COVERAGE "Enable code coverage" OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

find_package(SQLite3 REQUIRED)

set(SOURCES
    src/Flower.cpp
    src/Composition.cpp
    src/Order.cpp
    src/DatabaseManager.cpp
    src/AppController.cpp
)

set(HEADERS
    includes/Flower.h
    includes/Composition.h
    includes/Order.h
    includes/DatabaseManager.h
    includes/AuthSystem.h
    includes/AppController.h
)

add_library(flower_core STATIC ${SOURCES} ${HEADERS})

target_include_directories(flower_core PUBLIC includes)

target_link_libraries(flower_core PUBLIC SQLite::SQLite3)

add_executable(greenhouse_app src/main.cpp)

target_link_libraries(greenhouse_app PRIVATE flower_core)

if(MSVC)
    add_compile_options(/MDd)
endif()

if(ENABLE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(flower_core PRIVATE --coverage -fprofile-arcs -ftest-coverage)
        target_link_options(flower_core PRIVATE --coverage)

        target_compile_options(greenhouse_app PRIVATE --coverage -fprofile-arcs -ftest-coverage)
        target_link_options(greenhouse_app PRIVATE --coverage)
    endif()
endif()

enable_testing()
add_subdirectory(tests)