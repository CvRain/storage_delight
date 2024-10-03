//
// Created by cvrain on 24-9-23.
//

#include "schema_format.hpp"

#include "service/logger.hpp"
#include "models/schema_key.hpp"

namespace util_delight {
    std::optional<schema::User> SchemaFormat::try_to_user(const nlohmann::json &user_json) {
        try {
            return schema::User{.id = user_json[schema::key::bson_id].get<std::string>(),
                                .role = user_json[schema::key::role].get<int>(),
                                .user_name = user_json[schema::key::user_name].get<std::string>(),
                                .password = user_json[schema::key::password].get<std::string>(),
                                .create_time = user_json[schema::key::create_time].get<std::string>()};
        }
        catch (const std::exception &e) {
            service_delight::Logger::get_instance().log(service_delight::ConsoleLogger, spdlog::level::err,
                                                        "Failed format to user {}", e.what());
            return std::nullopt;
        }
    }
    std::optional<schema::User> SchemaFormat::try_to_user(const bsoncxx::document::value &json_value) {
        try {
            return schema::User{.id = json_value[schema::key::bson_id].get_oid().value.to_string(),
                                .role = json_value[schema::key::role].get_int32(),
                                .user_name = json_value[schema::key::user_name].get_string().value.data(),
                                .password = json_value[schema::key::password].get_string().value.data(),
                                .create_time = json_value[schema::key::create_time].get_string().value.data()};
        }
        catch (const std::exception &e) {
            service_delight::Logger::get_instance().log(service_delight::ConsoleLogger, spdlog::level::err,
                                                        "Failed to convert BSON to type user {}", e.what());
            return std::nullopt;
        }
    }
    std::optional<bsoncxx::document::value> SchemaFormat::to_bson(const schema::User &user) {
        bsoncxx::builder::basic::document doc;
        {
            doc.append(bsoncxx::builder::basic::kvp(schema::key::bson_id, user.id));
            doc.append(bsoncxx::builder::basic::kvp(schema::key::role, user.role));
            doc.append(bsoncxx::builder::basic::kvp(schema::key::user_name, user.user_name));
            doc.append(bsoncxx::builder::basic::kvp(schema::key::password, user.password));
            doc.append(bsoncxx::builder::basic::kvp(schema::key::create_time, user.create_time));
        }
        return doc.extract();
    }
} // namespace util_delight
