//
// Created by cvrain on 24-8-13.
//

#ifndef STORAGE_DELIGHT_SQLITE_SERVICE_H
#define STORAGE_DELIGHT_SQLITE_SERVICE_H

#include <sqlite_orm/sqlite_orm.h>
#include <string_view>
#include <filesystem>
#include <optional>
#include <spdlog/spdlog.h>

#include "models/db_schema.hpp"

inline static auto init_storage(const std::string_view &db_path) {
    std::filesystem::path path(db_path);
    //检查文件是否存在,如果不存在检查路径是否存在上级目录，如果存在上级目录则创建缺失的目录
    if (!std::filesystem::exists(path)) {
        if (!std::filesystem::exists(path.parent_path())) {
            std::filesystem::create_directories(path.parent_path());
        }
    }

    auto storage = sqlite_orm::make_storage(db_path.data(),
                                            schema::db_user_table,
                                            schema::db_minio_client_table);
    return storage;
}

using Storage = decltype(init_storage({}));
using StoragePtr = std::shared_ptr<Storage>;

class SqliteService {
public:
    explicit SqliteService(const std::string_view &db_path = "./db/storage_source.db");

    bool check_user_exist(const std::string_view &username);

    std::optional<schema::User> add_user(schema::BaseUser user);

    std::optional<schema::User> get_user_by_username(const std::string_view &user_name);


private:
    StoragePtr storage_instance;
};


#endif //STORAGE_DELIGHT_SQLITE_SERVICE_H
