# Dmytro Demchuk K-25
cmake_minimum_required(VERSION 3.16)
project(Variant13 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Threads REQUIRED)
add_executable(run_tests src/main.cpp src/fields.cpp)
target_link_libraries(run_tests PRIVATE Threads::Threads)
add_executable(create_input src/create_input.cpp)
