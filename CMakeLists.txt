# Dmytro Demchuk K-25
cmake_minimum_required(VERSION 3.16)
project(Variant13 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release)
endif()

find_package(Threads REQUIRED)

add_executable(lr_4_variant_13
  src/main.cpp
  src/fields.cpp
  src/operations.cpp
  src/benchmark.cpp)
target_link_libraries(lr_4_variant_13 PRIVATE Threads::Threads)

add_executable(create_input src/create_input.cpp)
