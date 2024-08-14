#ifndef STORAGE_DELIGHT_SQLITE_SERVICE_H
#define STORAGE_DELIGHT_SQLITE_SERVICE_H

#include <sqlite_orm/sqlite_orm.h>
#include <string>
#include <filesystem>
#include <optional>
#include <spdlog/spdlog.h>
#include <mutex>

#include "models/db_schema.hpp"

inline static auto init_storage(const std::string& db_path) {
    std::filesystem::path path(db_path);
    if (!std::filesystem::exists(path.parent_path())) {
        std::filesystem::create_directories(path.parent_path());
    }

    auto storage = sqlite_orm::make_storage(db_path,
                                            schema::db_user_table,
                                            schema::db_minio_client_table);
    return storage;
}

using Storage = decltype(init_storage({}));
using StoragePtr = std::unique_ptr<Storage>;

class SqliteService {
public:
    explicit SqliteService(const std::string_view& db_path = "./db/storage_source.db");

    bool check_user_exist(const std::string_view & username);

    std::optional<schema::User> add_user(const schema::BaseUser& user);

    std::optional<schema::User> get_user_by_username(const std::string_view& user_name);

    std::optional<schema::User> get_user_by_id(int id);

private:
    StoragePtr storage_instance;
};

class SqliteServiceProvider {
public:
    static SqliteServiceProvider& get_instance() {
        static std::mutex mutex;
        std::lock_guard<std::mutex> lock(mutex);
        static SqliteServiceProvider instance;
        return instance;
    }

    SqliteServiceProvider(SqliteServiceProvider const&) = delete;
    void operator=(SqliteServiceProvider const&) = delete;

    SqliteService& get_sqlite_service() {
        return sqlite_service;
    }

private:
    SqliteServiceProvider() : sqlite_service("./db/storage_source.db") {}
    SqliteService sqlite_service;
};

#endif //STORAGE_DELIGHT_SQLITE_SERVICE_H