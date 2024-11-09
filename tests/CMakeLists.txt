cmake_minimum_required(VERSION 3.17)

SET(TASK_NAME test)

project(${TASK_NAME} CXX)

include_directories(../include)
add_compile_options(-std=c++20 -lgtest -lpthread)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(${TASK_NAME} tests.cpp ../src/argument_parser.cpp)
target_link_libraries(${TASK_NAME} Threads::Threads ${GTEST_LIBRARIES} ${GMOCK_BOTH_LIBRARIES})
