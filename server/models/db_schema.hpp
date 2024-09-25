//
// Created by cvrain on 24-8-13.
//

#ifndef STORAGE_DELIGHT_DB_SCHEMA_HPP
#define STORAGE_DELIGHT_DB_SCHEMA_HPP

#include <string>
#include "schema_key.hpp"

namespace schema {
    enum UserRole : int {
        TypeNone = -1,
        TypeAdmin = 0,
        TypeUser = 1,
        TypeSourceAdmin,
        TypeBucketAdmin,
        TypeGroupAdmin,
        TypeRoleMax
    };

    struct BaseUser {
        UserRole role = UserRole::TypeNone;
        std::string user_name = "unknown";
        std::string password = "unknown";
    };

    struct User {
        std::string id{};
        int role = UserRole::TypeUser;
        std::string user_name;
        std::string password;
        std::string create_time;
    };


    struct MinioClient {
        int id;
        bool is_http;
        std::string name;
        std::string access_key;
        std::string secret_key;
        std::string link_url;
        std::string create_time;
    };
} // namespace schema

#endif // STORAGE_DELIGHT_DB_SCHEMA_HPP
