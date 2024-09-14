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

qt_standard_project_setup(REQUIRES 6.5)

set(SRC_CPP src/main.cpp)

set(RES_QML ui/src_ui.qrc)

set(SRC_UI ui/Main.qml
    ui/Layout/Login.qml
    ui/Theme/CatppuccinFrappe.qml
    ui/Theme/CatppuccinLatte.qml
    ui/Theme/CatppuccinMacchiato.qml
    ui/Theme/CatppuccinMocha.qml
    ui/Theme/ThemeProvider.qml)

qt_add_executable(appstorage_delight_compass
        ${SRC_CPP}
        ${RES_QML}
)

qt_add_qml_module(appstorage_delight_compass
        URI storage_delight_compass
        QML_FILES ${SRC_UI}
        VERSION 1.0
        RESOURCES ${RES_QML}
        RESOURCES qmldir
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
        cpprestsdk::cpprest cpprestsdk::cpprestsdk_zlib_internal cpprestsdk::cpprestsdk_boost_internal cpprestsdk::cpprestsdk_openssl_internal
        spdlog::spdlog
)

target_compile_features(appstorage_delight_compass PUBLIC cxx_std_20)


include(GNUInstallDirs)
install(TARGETS appstorage_delight_compass
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
