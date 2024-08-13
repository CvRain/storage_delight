//
// Created by cvrain on 24-8-13.
//

#include "sqlite_service.h"
#include "utils/string_encryption.h"

SqliteService::SqliteService(const std::string_view &db_path) {
    storage_instance = std::make_unique<Storage>(init_storage(db_path));
    storage_instance->sync_schema();
}

bool SqliteService::check_user_exist(const std::string_view &username) {
    return false;
}

std::optional<schema::User> SqliteService::get_user_by_username(const std::string_view &user_name) {
    return std::optional<schema::User>();
}

std::optional<schema::User> SqliteService::add_user(schema::BaseUser user) {
    auto full_user = schema::User{
            .role = user.role,
            .user_name = user.user_name,
            .password = util_delight::StringEncryption::sha256(user.password)
    };
    const auto result = storage_instance->insert(full_user);
    //TODO
    return full_user;
}
