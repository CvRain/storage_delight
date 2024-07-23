cmake_minimum_required(VERSION 3.20)
project(storage_delight_core VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(cpprestsdk CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

set(SRC_HEADER include/hello.hpp)
set(SRC_CPP src/hello.cpp)

add_library(storage_delight_core ${SRC_HEADER} ${SRC_CPP})

target_link_libraries(storage_delight_core PUBLIC
        cpprestsdk::cpprest
        cpprestsdk::cpprestsdk_zlib_internal
        cpprestsdk::cpprestsdk_boost_internal
        cpprestsdk::cpprestsdk_openssl_internal
        nlohmann_json::nlohmann_json)


target_include_directories(storage_delight_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)