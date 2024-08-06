cmake_minimum_required(VERSION 3.20)
project(storage_delight_app VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(ZLIB REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(miniocpp CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_executable(storage_delight_app src/app.cpp)

target_link_libraries(storage_delight_app storage_delight_core
        ZLIB::ZLIB
        nlohmann_json::nlohmann_json
        spdlog::spdlog
        miniocpp::miniocpp)