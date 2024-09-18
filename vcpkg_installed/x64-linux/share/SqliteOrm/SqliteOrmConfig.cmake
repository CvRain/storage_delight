include(CMakeFindDependencyMacro)
find_dependency(unofficial-sqlite3)

include(${CMAKE_CURRENT_LIST_DIR}/SqliteOrmTargets.cmake)
