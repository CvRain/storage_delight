cmake_minimum_required(VERSION 3.20)
project(storage_server VERSION 0.1.0)

include(CheckIncludeFileCXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(storage_server main.cc)

# ##############################################################################
# If you include the drogon source code locally in your project, use this method
# to add drogon 
# add_subdirectory(drogon) 
# target_link_libraries(storage_server PRIVATE drogon)
#
# and comment out the following lines
find_package(Drogon CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(SqliteOrm CONFIG REQUIRED)

target_link_libraries(storage_server PRIVATE Drogon::Drogon)
target_link_libraries(storage_server PRIVATE storage_delight_core)
target_link_libraries(storage_server PUBLIC nlohmann_json::nlohmann_json
        unofficial::sqlite3::sqlite3
        sqlite_orm::sqlite_orm
)

target_compile_features(storage_server PUBLIC cxx_std_20)

# ##############################################################################


aux_source_directory(controllers CTL_SRC)
aux_source_directory(filters FILTER_SRC)
aux_source_directory(plugins PLUGIN_SRC)
aux_source_directory(models MODEL_SRC)
aux_source_directory(utils UTILS_SRC)
aux_source_directory(service SERVICE_SRC)

drogon_create_views(storage_server ${CMAKE_CURRENT_SOURCE_DIR}/views
        ${CMAKE_CURRENT_BINARY_DIR})
# use the following line to create views with namespaces.
# drogon_create_views(storage_server ${CMAKE_CURRENT_SOURCE_DIR}/views
#                     ${CMAKE_CURRENT_BINARY_DIR} TRUE)
# use the following line to create views with namespace CHANGE_ME prefixed
# and path namespaces.
# drogon_create_views(storage_server ${CMAKE_CURRENT_SOURCE_DIR}/views
#                     ${CMAKE_CURRENT_BINARY_DIR} TRUE CHANGE_ME)

target_include_directories(storage_server
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/models)
target_sources(storage_server
        PRIVATE
        ${SRC_DIR}
        ${CTL_SRC}
        ${FILTER_SRC}
        ${PLUGIN_SRC}
        ${MODEL_SRC}
        ${UTILS_SRC}
        ${SERVICE_SRC})
# ##############################################################################
# uncomment the following line for dynamically loading views 
# set_property(TARGET storage_server PROPERTY ENABLE_EXPORTS ON)

# ##############################################################################

add_subdirectory(test)
