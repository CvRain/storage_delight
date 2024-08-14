//
// Created by cvrain on 24-8-13.
//

#include "sqlite_service.h"
#include "utils/string_encryption.h"
#include "utils/date.h"

SqliteService::SqliteService(const std::string_view &db_path) {
    storage_instance = std::make_unique<Storage>(init_storage(db_path.data()));
    storage_instance->sync_schema();
}

bool SqliteService::check_user_exist(const std::string_view &username) {
    return !get_user_by_username(username)->user_name.empty();
}

std::optional<schema::User> SqliteService::get_user_by_username(const std::string_view &user_name) {
    spdlog::info("SqliteService::get_user_by_username: user_name:{}", user_name);
    const auto result = storage_instance->get_optional<schema::User>(
            sqlite_orm::where(sqlite_orm::c(&schema::User::user_name) == user_name));
    spdlog::info("SqliteService::get_user_by_username: user_name:{} result:{}", user_name, result->user_name);
    return result;
}

std::optional<schema::User> SqliteService::add_user(const schema::BaseUser &user) {
    const auto id = storage_instance->insert(schema::User{
            .role = user.role,
            .user_name = user.user_name,
            .password = util_delight::StringEncryption::sha256(user.password),
            .create_time = std::to_string(util_delight::Date::get_current_timestamp_32())
    });
    auto fully_user = get_user_by_id(id);
    return fully_user;
}

std::optional<schema::User> SqliteService::get_user_by_id(int id) {
    try {
        return storage_instance->get<schema::User>(sqlite_orm::where(sqlite_orm::c(&schema::User::id) == id));
    } catch (const std::system_error &e) {
        spdlog::error("SqliteService::get_user_by_id: {}", e.what());
    } catch (...) {
        spdlog::error("SqliteService::get_user_by_id: unknown error");
    }
    return std::nullopt;
}
