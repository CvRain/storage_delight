//
// Created by cvrain on 24-9-22.
//

#include "user_service.hpp"

#include "schema_key.hpp"
#include "service/logger.hpp"
#include "utils/string.hpp"

namespace service_delight {
    void UserService::init() {
        Logger::get_instance().log(ConsoleLogger, "UserService init");

        user_collection = MongoProvider::get_instance().get_collection(schema::key::collection::user);
    }

    schema::result<schema::DbUser, std::string> UserService::add_user(schema::DbUser *user,mongocxx::client_session* session) {
        Logger::get_instance().log(ConsoleLogger, "Enter UserService::add_user");
        try {

            if (user == nullptr) {
                Logger::get_instance().log(ConsoleLogger | BasicLogger, "UserService::add_user received null user");
                return std::make_pair(std::nullopt, "User is null");
            }

            if (user->name.empty()) {
                Logger::get_instance().log(ConsoleLogger | BasicLogger, "UserService::add_user received null user");
                return std::make_pair(std::nullopt, "User name is empty");
            }

            if (user_is_exist(user->name)) {
                Logger::get_instance().log(ConsoleLogger | BasicLogger, "UserService::add_user {} already exist",
                                           user->name);
                return std::make_pair(std::nullopt, "User already exist");
            }

            const auto user_document = user->get_document();
            const auto result = user_collection.insert_one(user_document.view());

            if (!result.has_value()) {
                Logger::get_instance().log(ConsoleLogger | BasicLogger, "UserService::add_user failed");
                return std::make_pair(std::nullopt, "UserService::add_user failed");
            }
            user->id = result.value().inserted_id().get_oid().value;
        }
        catch (const std::exception &e) {
            Logger::get_instance().log(ConsoleLogger | BasicLogger, "UserService::add_user failed: {}", e.what());
            return std::make_pair(std::nullopt, e.what());
        }
        return std::make_pair(*user, "");
    }

    schema::result<bsoncxx::document::value, std::string>
    UserService::add_user_v2(const bsoncxx::document::value &value) {
        Logger::get_instance().log(ConsoleLogger, "Enter UserService::add_user_v2");

        try {
            const auto user_name = value[schema::key::name].get_string().value;
            if (user_is_exist(user_name.data())) {
                Logger::get_instance().log(ConsoleLogger | BasicLogger, "UserService::add_user_v2 {} already exist",
                                           user_name);
                return std::make_pair(std::nullopt, "User already exist");
            }

            const auto insert_result = user_collection.insert_one(value.view());
            if (!insert_result.has_value()) {
                Logger::get_instance().log(ConsoleLogger | BasicLogger, "UserService::add_user_v2 failed");
                return std::make_pair(std::nullopt, "UserService::add_user_v2 failed");
            }

            const auto user = get_user_by_id(insert_result.value().inserted_id().get_oid().value);
            return user;
        }
        catch (const std::exception &e) {
            Logger::get_instance().log(ConsoleLogger | BasicLogger, "UserService::add_user_v2 failed: {}", e.what());
            return std::make_pair(std::nullopt, e.what());
        }
    }

    schema::result<bsoncxx::document::value, std::string> UserService::get_user_by_id(const bsoncxx::oid &id) {
        Logger::get_instance().log(ConsoleLogger, "Enter UserService::get_user_by_id");
        try {
            const auto user_document = user_collection.find_one(make_document(kvp(schema::key::bson_id, id)));
            if (!user_document.has_value()) {
                Logger::get_instance().log(ConsoleLogger | BasicLogger, "UserService::get_user_by_id {} not found",
                                           id.to_string());
                return std::make_pair(std::nullopt, "User not found");
            }
            return std::make_pair(user_document, "");
        }
        catch (const std::exception &e) {
            Logger::get_instance().log(ConsoleLogger | BasicLogger, "UserService::get_user_by_id failed: {}", e.what());
            return std::make_pair(std::nullopt, e.what());
        }
    }

    schema::result<bsoncxx::document::value, std::string> UserService::get_user_by_name(const std::string &user_name) {
        Logger::get_instance().log(ConsoleLogger, "Enter UserService::get_user_by_name");
        try {
            const auto user_document = user_collection.find_one(make_document(kvp(schema::key::name, user_name)));
            if (!user_document.has_value()) {
                Logger::get_instance().log(ConsoleLogger | BasicLogger, "UserService::get_user_by_name {} not found",
                                           user_name);
                return std::make_pair(std::nullopt, "User not found");
            }
            return std::make_pair(user_document, "");
        }
        catch (const std::exception &e) {
            Logger::get_instance().log(ConsoleLogger | BasicLogger, "UserService::get_user_by_name failed: {}",
                                       e.what());
            return std::make_pair(std::nullopt, e.what());
        }
    }

    bool UserService::user_is_exist(const std::string &user_name) {
        // schema::key::name is "name"
        Logger::get_instance().log(ConsoleLogger, "Enter UserService::user_is_exist");
        return user_collection.find_one(make_document(kvp(schema::key::name, user_name))).has_value();
    }

    bool UserService::user_is_exist(const bsoncxx::oid &id) {
        Logger::get_instance().log(ConsoleLogger, "Enter UserService::user_is_exist");
        try {
            const auto filter = make_document(kvp(schema::key::bson_id, id));
            if (const auto result = user_collection.find_one(filter.view());result) {
                Logger::get_instance().log(ConsoleLogger | BasicLogger, spdlog::level::debug,
                    "UserService::user_is_exist {} found", id.to_string());
                return true;
            }
            Logger::get_instance().log(ConsoleLogger | BasicLogger,  spdlog::level::debug,
                "UserService::user_is_exist {} not found", id.to_string());
            return false;
        }
        catch (const std::exception &e) {
            Logger::get_instance().log(ConsoleLogger | BasicLogger, spdlog::level::err,
                                       "UserService::user_is_exist failed: {}", e.what());
            return false;
        }
    }

    bool UserService::admin_is_exist() {
        Logger::get_instance().log(ConsoleLogger, "Enter UserService::admin_is_exist");
        try {
            const auto result = user_collection.find_one(make_document(kvp(schema::key::user_role, schema::TypeAdmin)));
            return result.has_value();
        }
        catch (const std::exception &e) {
            Logger::get_instance().log(ConsoleLogger | BasicLogger, spdlog::level::err,
                                       "UserService::admin_is_exist failed: {}", e.what());
            return false;
        }
    }
    auto UserService::list_user_ids() -> schema::result<std::vector<bsoncxx::oid>, std::string> {
        Logger::get_instance().log(ConsoleLogger, "Enter UserService::list_user_ids");
        try {
            auto cursor = user_collection.find(make_document());
            std::vector<bsoncxx::oid> ids;
            for (auto doc: cursor) {
                ids.emplace_back(doc[schema::key::bson_id].get_oid().value);
            }
            return std::make_pair(ids, "");
        }
        catch (const std::exception &e) {
            Logger::get_instance().log(ConsoleLogger | BasicLogger, spdlog::level::err,
                                       "UserService::list_user_ids failed: {}", e.what());
            return std::make_pair(std::nullopt, e.what());
        }
    }

    auto UserService::list_all_users() -> schema::result<std::vector<bsoncxx::document::value>, std::string> {
        Logger::get_instance().log(ConsoleLogger, "Enter UserService::list_all_users");
        try {
            auto cursor = user_collection.find(make_document());
            std::vector<bsoncxx::document::value> users;
            for (auto doc: cursor) {
                users.emplace_back(doc);
            }
            return std::make_pair(users, "");
        }
        catch (const std::exception &e) {
            Logger::get_instance().log(ConsoleLogger | BasicLogger, spdlog::level::err,
                                       "UserService::list_all_users failed: {}", e.what());
            return std::make_pair(std::nullopt, e.what());
        }
    }
    auto UserService::delete_user(bsoncxx::oid user_id) -> schema::result<bool, std::string> {
        Logger::get_instance().log(ConsoleLogger, "Enter UserService::delete_user");
        try {
            const auto filter = make_document(kvp(schema::key::bson_id, user_id));
            if (const auto result = user_collection.delete_one(filter.view());
                result && result.value().deleted_count() >0) {
                return std::make_pair(true, "");
            }
            return std::make_pair(false, "User not found");
        }catch (const std::exception &e) {
            Logger::get_instance().log(ConsoleLogger | BasicLogger, spdlog::level::err,
                                   "UserService::delete_user failed: {}", e.what());
            return std::make_pair(false, e.what());
        }
    }
} // namespace service_delight
