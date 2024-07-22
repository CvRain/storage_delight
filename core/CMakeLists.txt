cmake_minimum_required(VERSION 3.20)
project(storage_delight_core VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(storage_delight_core src/hello.cpp)

target_include_directories(storage_delight_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)