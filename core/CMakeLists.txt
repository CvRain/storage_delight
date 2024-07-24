cmake_minimum_required(VERSION 3.20)
project(storage_delight_core VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(nlohmann_json CONFIG REQUIRED)
find_package(miniocpp CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

set(SRC_HEADER
        include/client.hpp
)

set(SRC_CPP
        src/client.cpp
)

add_library(storage_delight_core ${SRC_HEADER} ${SRC_CPP})

target_link_libraries(storage_delight_core PUBLIC
        nlohmann_json::nlohmann_json
        miniocpp::miniocpp
        spdlog::spdlog)


target_include_directories(storage_delight_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)