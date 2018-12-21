cmake_minimum_required(VERSION 3.6.2)

project(test_regex)

add_definitions(-Wall -std=c++17 -O3)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME})