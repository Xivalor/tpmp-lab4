include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)

FetchContent_MakeAvailable(googletest)

# Тесты
add_executable(test_flowers test_flowers.cpp)
target_link_libraries(test_flowers PRIVATE gtest_main gtest flower_core)
target_include_directories(test_flowers PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_executable(test_compositions test_compositions.cpp)
target_link_libraries(test_compositions PRIVATE gtest_main gtest flower_core)
target_include_directories(test_compositions PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_executable(test_orders test_orders.cpp)
target_link_libraries(test_orders PRIVATE gtest_main gtest flower_core)
target_include_directories(test_orders PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_test(NAME FlowersTest COMMAND test_flowers)
add_test(NAME CompositionsTest COMMAND test_compositions)
add_test(NAME OrdersTest COMMAND test_orders)
