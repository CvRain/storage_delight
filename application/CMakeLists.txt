cmake_minimum_required(VERSION 3.20)

project(storage_delight_compass VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick)
find_package(spdlog CONFIG REQUIRED)
find_package(cpprestsdk CONFIG REQUIRED)

if(Win32)
    find_package(fmt CONFIG REQUIRED)
endif ()

qt_standard_project_setup(REQUIRES 6.5)

include_directories(src)

set(SRC_CPP src/main.cpp
        src/schema/theme.hpp
        src/theme_provider.cpp
        src/theme_provider.hpp)

set(SRC_UI ui/Main.qml
    ui/Layout/Login.qml
)

qt_add_executable(appstorage_delight_compass
        ${SRC_CPP}
        res/resource.qrc
)

set_source_files_properties(${SRC_SINGLETON_QML} PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

qt_add_qml_module(appstorage_delight_compass
        URI storage_delight_compass
        QML_FILES ${SRC_UI} ${SRC_SINGLETON_QML}
        VERSION 1.0
        RESOURCES res/resource.qrc
    )

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appstorage_delight_compass PROPERTIES
        #    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appstorage_delight_compass
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
)

target_link_libraries(appstorage_delight_compass
        PUBLIC Qt6::Quick
        spdlog::spdlog
)

if(UNIX)
    target_link_libraries(appstorage_delight_compass PUBLIC
            cpprestsdk::cpprest
            cpprestsdk::cpprestsdk_zlib_internal
            cpprestsdk::cpprestsdk_boost_internal
            cpprestsdk::cpprestsdk_openssl_internal
    )
elseif (Win32)
    target_link_libraries(appstorage_delight_compass PUBLIC
            fmt::fmt
            cpprestsdk::cpprest
            cpprestsdk::cpprestsdk_zlib_internal
            cpprestsdk::cpprestsdk_brotli_internal
    )
endif ()

target_compile_features(appstorage_delight_compass PUBLIC cxx_std_20)


include(GNUInstallDirs)
install(TARGETS appstorage_delight_compass
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
