//
// Created by cvrain on 24-8-13.
//

#include "sqlite_service.h"
#include "utils/string.h"
#include "utils/date.h"

SqliteService::SqliteService(const std::string_view& db_path) {
	storage_instance = std::make_unique<storage>(init_storage(db_path.data()));
	storage_instance->sync_schema();
}

bool SqliteService::check_user_exist(const std::string_view& username) {
	return !get_user_by_username(username).has_value();
}

std::optional<schema::User> SqliteService::get_user_by_username(const std::string_view& user_name) {
	spdlog::info("SqliteService::get_user_by_username: user_name:{}", user_name);
	try {
		if (const auto users = storage_instance->get_all<schema::User>(
			sqlite_orm::where(sqlite_orm::c(&schema::User::user_name) == user_name.data())); !users.empty()) {
			return users.at(0);
		}
	}
	catch (const std::exception& e) {
		spdlog::error("SqliteService::get_user_by_username: {}", e.what());
		return std::nullopt;
	}
	return std::nullopt;
}

std::optional<schema::User> SqliteService::add_user(const schema::BaseUser& user) {
	const auto id = storage_instance->insert(schema::User{
			.role = user.role,
			.user_name = user.user_name,
			.password = util_delight::StringEncryption::sha256(user.password),
			.create_time = std::to_string(util_delight::Date::get_current_timestamp_32())
		});
	auto fully_user = get_user_by_id(id);
	return fully_user;
}

std::optional<schema::User> SqliteService::get_user_by_id(const int id) {
	try {
		return storage_instance->get<schema::User>(sqlite_orm::where(sqlite_orm::c(&schema::User::id) == id));
	}
	catch (const std::system_error& e) {
		spdlog::error("SqliteService::get_user_by_id: {}", e.what());
	}
	catch (...) {
		spdlog::error("SqliteService::get_user_by_id: unknown error");
	}
	return std::nullopt;
}

int SqliteService::get_admin_count() {
	spdlog::info("SqliteService::get_admin_count");
	const auto admin_count = storage_instance->count<schema::User>(
		sqlite_orm::where(
			sqlite_orm::c(&schema::User::role) == 0)
	);
	return admin_count;
}
