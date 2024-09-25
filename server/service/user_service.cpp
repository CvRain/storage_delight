//
// Created by cvrain on 24-9-22.
//

#include "user_service.hpp"

#include "schema_format.hpp"
#include "service/logger.hpp"
#include "utils/string.hpp"

namespace service_delight {
    void UserService::init() {
        Logger::get_instance().log(ConsoleLogger, "UserService init");

        user_collection = MongoService::get_instance().get_collection("user");
    }

    schema::User UserService::add_user(const schema::BaseUser &user) {
        Logger::get_instance().log(ConsoleLogger, "Enter UserService::add_user");

        auto create_time = std::to_string(util_delight::Date::get_current_timestamp_32());
        const auto doc_value = make_document(kvp("user_name", user.user_name),
                                             kvp("password", util_delight::StringEncryption::sha256(user.password)),
                                             kvp("role", user.role), kvp("create_time", create_time));

        const auto result = user_collection.insert_one(doc_value.view());

        if (!result) {
            Logger::get_instance().log(ConsoleLogger, "UserService::add_user failed");
            return schema::User{};
        }
        const auto object_id = result->inserted_id().get_oid().value;
        Logger::get_instance().log(ConsoleLogger | BasicLogger, "UserService::add_user {}", object_id.to_string());

        return schema::User{.id = object_id.to_string(),
                            .role = user.role,
                            .user_name = user.user_name,
                            .password = user.password,
                            .create_time = std::move(create_time)};
    }

    bool UserService::user_is_exist(const std::string &user_name) {
        Logger::get_instance().log(ConsoleLogger, "Enter UserService::user_is_exist");

        return user_collection.find_one(make_document(kvp(schema::key::user_name, user_name))).has_value();
    }

    bool UserService::admin_is_exist() {
        Logger::get_instance().log(ConsoleLogger, "Enter UserService::admin_is_exist");
        return user_collection.find_one(make_document(kvp(schema::key::role, schema::UserRole::TypeAdmin))).has_value();
    }

    std::optional<schema::User> UserService::get_user_by_name(const std::string &user_name) {
        Logger::get_instance().log(ConsoleLogger, "Enter UserService::get_user");

        const auto result = user_collection.find_one(make_document(kvp(schema::key::user_name, user_name)));
        if (!result.has_value()) {
            return std::nullopt;
        }
        const auto user = util_delight::SchemaFormat::try_to_user(result.value());
        return user;
    }

    /***
     * @brief  根据std::string id获取用户信息
     * @param id 接受std::string类型的id, 会在内部转换为bsoncxx::oid
     * @return  std::optional<schema::User>
     */
    std::optional<schema::User> UserService::get_user_by_id(const std::string &id) {
        Logger::get_instance().log(ConsoleLogger, "Enter UserService::get_user_by_id");

        if (const auto result = user_collection.find_one(make_document(kvp(schema::key::bson_id, bsoncxx::oid{id})));
            result.has_value()) {
            Logger::get_instance().log(ConsoleLogger | BasicLogger, "UserService::get_user_by_id {}", id);
            return util_delight::SchemaFormat::try_to_user(result.value());
        }
        return std::nullopt;
    }

    std::optional<schema::User> UserService::remove_user(const std::string &user_name) {
        Logger::get_instance().log(ConsoleLogger, "Enter UserService::remove_user");

        if (const auto result = user_collection.find_one(make_document(kvp(schema::key::user_name, user_name)));
            result.has_value()) {
            user_collection.delete_one(make_document(kvp("user_name", user_name)));
            Logger::get_instance().log(ConsoleLogger | BasicLogger | DailyLogger, "UserService::remove_user {}",
                                       user_name);
            return util_delight::SchemaFormat::try_to_user(result.value());
        }
        return std::nullopt;
    }
} // namespace service_delight
