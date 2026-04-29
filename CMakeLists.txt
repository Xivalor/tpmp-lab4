cmake_minimum_required(VERSION 3.10)
project(tpmp-lab4 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ENABLE_COVERAGE "Enable code coverage" OFF)

set(SOURCES
    src/Flower.cpp
    src/Composition.cpp
    src/Order.cpp
)

set(HEADERS
    src/Flower.h
    src/Composition.h
    src/Order.h
)

add_library(flower_core STATIC ${SOURCES} ${HEADERS})
target_include_directories(flower_core PUBLIC src)

add_executable(tpmp-lab4 src/main.cpp)
target_link_libraries(tpmp-lab4 PRIVATE flower_core)

if(WIN32)
    add_compile_options(/MDd)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MDd")
endif()

if(ENABLE_COVERAGE)
    target_compile_options(flower_core PRIVATE -coverage -fprofile-arcs -ftest-coverage)
    target_link_options(flower_core PRIVATE -coverage -fprofile-arcs -ftest-coverage)
    
    # Добавить флаги для исполняемого файла
    target_compile_options(tpmp-lab4 PRIVATE -coverage -fprofile-arcs -ftest-coverage)
    target_link_options(tpmp-lab4 PRIVATE -coverage -fprofile-arcs -ftest-coverage)
endif()

enable_testing()
add_subdirectory(tests)
