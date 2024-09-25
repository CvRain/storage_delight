//
// Created by cvrain on 24-9-23.
//

#ifndef SCHEMA_KEY_HPP
#define SCHEMA_KEY_HPP

namespace schema::key {
    constexpr std::string_view common_id = "id";
    constexpr std::string_view bson_id = "_id";
    constexpr std::string_view user_name = "user_name";
    constexpr std::string_view password = "password";
    constexpr std::string_view role = "role";
    constexpr std::string_view create_time = "create_time";
    constexpr std::string_view update_time = "update_time";
}

#endif //SCHEMA_KEY_HPP
