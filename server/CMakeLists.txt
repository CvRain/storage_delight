cmake_minimum_required(VERSION 3.20)
project(storage_server VERSION 0.1.0)

include(CheckIncludeFileCXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
message(STATUS "VCPKG Installed Directory: ${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}")

# ##############################################################################

set(CTL_SRC
        controllers/api_bucket.cc
        controllers/api_bucket.h
        controllers/api_hello.cc
        controllers/api_hello.h
        controllers/api_object.cc
        controllers/api_object.h
        controllers/api_user.cc
        controllers/api_user.h
        controllers/api_storage_source.cc
        controllers/api_storage_source.h
        controllers/api_group.h
        controllers/api_group.cc)


set(FILTER_SRC
        filters/user_filter.hpp
        filters/user_filter.cpp
        filters/user_exist.cpp
        filters/user_exist.hpp
)
aux_source_directory(plugins PLUGIN_SRC)

set(MODEL_SRC
        models/db_schema.hpp
        models/drogon_specialization.hpp
        models/nlohmann_json_request.hpp
        models/type.hpp
        models/nlohmann_json_response.hpp
        models/schema_key.hpp
        models/basic_value.hpp
        models/db_schema.cpp
)

set(UTILS_SRC
        utils/date.h
        utils/format.h
        utils/string.hpp
        utils/string.cpp
        utils/singleton_prototype.hpp
        utils/item.h
)

set(SERVICE_SRC
        service/mongo_service.cpp
        service/mongo_service.hpp
        service/logger.cpp
        service/logger.hpp
        service/user_service.cpp
        service/user_service.hpp
        service/group_service.cpp
        service/group_service.hpp
        service/log_service.cpp
        service/log_service.hpp
        service/storage_service.cpp
        service/storage_service.hpp
)

include_directories(${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include)

add_executable(storage_server
        main.cc
        ${CTL_SRC}
        ${FILTER_SRC}
        ${PLUGIN_SRC}
        ${MODEL_SRC}
        ${UTILS_SRC}
        ${SERVICE_SRC}
)

# ##############################################################################
# If you include the drogon source code locally in your project, use this method
# to add drogon 
# add_subdirectory(drogon) 
# target_link_libraries(storage_server PRIVATE drogon)
#
# and comment out the following lines
find_package(fmt CONFIG REQUIRED)
find_package(Drogon CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(mongoc-1.0 CONFIG REQUIRED)
find_package(bsoncxx CONFIG REQUIRED)
find_package(mongocxx CONFIG REQUIRED)
find_path(ZPP_BITS_INCLUDE_DIRS "zpp_bits.h")

target_link_libraries(storage_server PRIVATE Drogon::Drogon)
target_link_libraries(storage_server PRIVATE storage_delight_core)
target_link_libraries(storage_server PUBLIC 
        nlohmann_json::nlohmann_json
        fmt::fmt
        $<IF:$<TARGET_EXISTS:mongo::mongoc_shared>,mongo::mongoc_shared,mongo::mongoc_static>
        $<IF:$<TARGET_EXISTS:mongo::bsoncxx_static>,mongo::bsoncxx_static,mongo::bsoncxx_shared>
        $<IF:$<TARGET_EXISTS:mongo::mongocxx_static>,mongo::mongocxx_static,mongo::mongocxx_shared>
)

target_compile_features(storage_server PUBLIC cxx_std_20)


drogon_create_views(storage_server ${CMAKE_CURRENT_SOURCE_DIR}/views
        ${CMAKE_CURRENT_BINARY_DIR})

target_include_directories(storage_server
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/models
        ${ZPP_BITS_INCLUDE_DIRS}
)


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
