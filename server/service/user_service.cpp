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

        user_collection = MongoService::get_instance().get_collection(schema::key::collection::user);
    }

    schema::result<schema::DbUser, std::string> UserService::add_user(schema::DbUser *user) {
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

    schema::result<nlohmann::json, std::string> UserService::add_user_v2(const bsoncxx::document::value &value) {
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

    schema::result<nlohmann::json, std::string> UserService::get_user_by_id(const bsoncxx::oid &id) {
        Logger::get_instance().log(ConsoleLogger, "Enter UserService::get_user_by_id");
        try {
            const auto user_document = user_collection.find_one(make_document(kvp(schema::key::bson_id, id)));
            if (!user_document.has_value()) {
                Logger::get_instance().log(ConsoleLogger | BasicLogger, "UserService::get_user_by_id {} not found",
                                           id.to_string());
                return std::make_pair(std::nullopt, "User not found");
            }
            return std::make_pair(bsoncxx::to_json(user_document.value().view()), "");
        }
        catch (const std::exception &e) {
            Logger::get_instance().log(ConsoleLogger | BasicLogger, "UserService::get_user_by_id failed: {}", e.what());
            return std::make_pair(std::nullopt, e.what());
        }
    }

    schema::result<nlohmann::json, std::string> UserService::get_user_by_name(const std::string &user_name) {
        Logger::get_instance().log(ConsoleLogger, "Enter UserService::get_user_by_name");
        try {
            const auto user_document = user_collection.find_one(make_document(kvp(schema::key::name, user_name)));
            if (!user_document.has_value()) {
                Logger::get_instance().log(ConsoleLogger | BasicLogger, "UserService::get_user_by_name {} not found",
                                           user_name);
                return std::make_pair(std::nullopt, "User not found");
            }
            return std::make_pair(bsoncxx::to_json(user_document.value().view()), "");
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
        try
        {
            const auto result = user_collection.find_one(make_document(kvp(schema::key::bson_id, id)));
            return result.has_value();
        }
        catch(const std::exception& e)
        {
            Logger::get_instance().log(ConsoleLogger | BasicLogger, spdlog::level::err,
             "UserService::user_is_exist failed: {}", e.what());
            return false;
        }
        
    }

    bool UserService::admin_is_exist() {
        Logger::get_instance().log(ConsoleLogger, "Enter UserService::admin_is_exist");
        try{
            const auto result = user_collection.find_one(make_document(kvp(schema::key::user_role, schema::TypeAdmin)));
            return result.has_value();
        }catch(const std::exception &e){
            Logger::get_instance().log(ConsoleLogger | BasicLogger, spdlog::level::err,
             "UserService::admin_is_exist failed: {}", e.what());
            return false;
        }
    }
} // namespace service_delight
