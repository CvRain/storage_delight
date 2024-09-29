//
// Created by cvrain on 24-8-13.
//

#ifndef STORAGE_DELIGHT_DB_SCHEMA_HPP
#define STORAGE_DELIGHT_DB_SCHEMA_HPP

#include <string>
#include <utility>
#include <map>
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

    struct DbUser {
        std::string id;
        std::string name;
        std::string password;
        std::string group_id;
        uint32_t create_time;
        uint32_t update_time;
    };

    struct DbBucket {
        std::string id;
        std::string data_source;
        std::string bucket_name;
        std::string group_id;
        std::string permission;
        std::vector<std::string> tags;
        uint32_t create_time;
        uint32_t update_time;
    };

    struct DbDataSource {
        std::string id;
        std::string name;
        std::string url;
        std::string access_key;
        std::string secret_key;
        std::string create_time;
    };

    struct  DbGroup {
        std::string id;
        std::string name;
        std::string owner_id;
        std::vector<std::string> members_id;
        std::vector<std::string> bucket_group_id;
        uint32_t create_time;
        uint32_t update_time;
    };

    /**
     * @param allow_actions: key: action name, value: user id
     * e.g. "read":["0001","0002","0003"]
     */
    struct DbPermission {
        std::string id;
        std::string bucket_id;
        std::string name;
        std::map<std::string, std::vector<std::string>> allow_actions;
        std::string description;
        std::string create_time;
        std::string update_time;
    };

    struct DbOperationLog {
        std::string id;
        std::string user_id;
        std::string action;
        std::string bucket_name;
        std::string object_name;
        uint32_t timestamp;
        std::string previous_state;
        std::string current_state;
        std::string description;
    };
} // namespace schema

#endif // STORAGE_DELIGHT_DB_SCHEMA_HPP
