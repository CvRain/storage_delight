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

        if(user == nullptr) {
            Logger::get_instance().log(ConsoleLogger | BasicLogger, "UserService::add_user received null user");
            return std::make_pair(std::nullopt, "User is null");
        }

        if(user->name.empty()) {
            Logger::get_instance().log(ConsoleLogger | BasicLogger, "UserService::add_user received null user");
            return std::make_pair(std::nullopt, "User name is empty");
        }

        if (!user_collection) {
            Logger::get_instance().log(ConsoleLogger | BasicLogger, "UserService::add_user user_collection is uninitialized");
            return std::make_pair(std::nullopt, "User collection is uninitialized");
        }

        try {
            if (user_is_exist(user->name)) {
                Logger::get_instance().log(ConsoleLogger | BasicLogger,
                                           "UserService::add_user {} already exist", user->name);
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

    bool UserService::user_is_exist(const std::string &user_name) {
        //schema::key::name is "name"
        Logger::get_instance().log(ConsoleLogger, "Enter UserService::user_is_exist");
        return user_collection.find_one(make_document(kvp(schema::key::name, user_name))).has_value();
    }

    bool UserService::admin_is_exist() {
        Logger::get_instance().log(ConsoleLogger, "Enter UserService::admin_is_exist");
        return user_collection.find_one(make_document(kvp(schema::key::user_role, schema::UserRole::TypeAdmin))).
                               has_value();
    }

    // std::optional<schema::User> UserService::get_user_by_name(const std::string &user_name) {
    //     Logger::get_instance().log(ConsoleLogger, "Enter UserService::get_user");
    //
    //     const auto result = user_collection.find_one(make_document(kvp(schema::key::name, user_name)));
    //     if (!result.has_value()) {
    //         return std::nullopt;
    //     }
    //     const auto user = util_delight::SchemaFormat::try_to_user(result.value());
    //     return user;
    // }

    /***
     * @brief  根据std::string id获取用户信息
     * @param id 接受std::string类型的id, 会在内部转换为bsoncxx::oid
     * @return  std::optional<schema::User>
     */
    // std::optional<schema::User> UserService::get_user_by_id(const std::string &id) {
    //     Logger::get_instance().log(ConsoleLogger, "Enter UserService::get_user_by_id");
    //
    //     if (const auto result = user_collection.find_one(make_document(kvp(schema::key::bson_id, bsoncxx::oid{id})));
    //         result.has_value()) {
    //         Logger::get_instance().log(ConsoleLogger | BasicLogger, "UserService::get_user_by_id {}", id);
    //         return util_delight::SchemaFormat::try_to_user(result.value());
    //     }
    //     return std::nullopt;
    // }
    //
    // std::optional<schema::User> UserService::remove_user(const std::string &user_name) {
    //     Logger::get_instance().log(ConsoleLogger, "Enter UserService::remove_user");
    //
    //     if (const auto result = user_collection.find_one(make_document(kvp(schema::key::name, user_name)));
    //         result.has_value()) {
    //         user_collection.delete_one(make_document(kvp("user_name", user_name)));
    //         Logger::get_instance().log(ConsoleLogger | BasicLogger | DailyLogger, "UserService::remove_user {}",
    //                                    user_name);
    //         return util_delight::SchemaFormat::try_to_user(result.value());
    //     }
    //     return std::nullopt;
    // }
} // namespace service_delight
