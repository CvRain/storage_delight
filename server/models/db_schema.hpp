//
// Created by cvrain on 24-8-13.
//

#ifndef STORAGE_DELIGHT_DB_SCHEMA_HPP
#define STORAGE_DELIGHT_DB_SCHEMA_HPP

#include <string>
#include <map>
#include <vector>
#include <bsoncxx/builder/basic/document.hpp>

#include "utils/date.h"
#include "utils/format.h"

namespace schema {
    enum UserRole : int {
        TypeNone = -1,
        TypeAdmin = 0,
        TypeUser = 1,
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

    class MongoDocument {
    public:
        MongoDocument() {
            id = bsoncxx::oid{}.to_string();
            create_time = util_delight::Date::get_current_timestamp_32();
        };
        virtual ~MongoDocument() = default;

        std::string id{};
        int32_t create_time{};

        virtual bsoncxx::document::view get_document() = 0;
    };

    class DbUser final : public MongoDocument {
    public:
        bsoncxx::document::view get_document() override;

        std::string name;
        std::string password;
        std::string group_id;
        int32_t role = UserRole::TypeUser;
        int32_t update_time;
    };

    class DbBucket final : public MongoDocument {
    public:
        bsoncxx::document::view get_document() override;

        std::string data_source;
        std::string bucket_name;
        std::string group_id;
        std::string permission_id;
        std::vector<std::string> tags;
        int32_t update_time;

    };

    class DbDataSource final : public MongoDocument {
    public:
        bsoncxx::document::view get_document() override;

        std::string id;
        std::string name;
        std::string url;
        std::string access_key;
        std::string secret_key;
        std::string create_time;
    };

    class DbGroup final : public MongoDocument {
    public:
        bsoncxx::document::view get_document() override;

        std::string id;
        std::string name;
        std::string owner_id;
        std::vector<std::string> members_id;
        std::vector<std::string> bucket_group_id;
        int32_t create_time;
        int32_t update_time;
    };

    /**
     * @param allow_actions: key: action name, value: user id
     * e.g. "allow_read":["0001","0002","0003"]
     */
    class DbPermission final : public MongoDocument {
    public:
        bsoncxx::document::view get_document() override;


        std::string id;
        std::string bucket_id;
        std::string name;
        std::map<std::string, std::vector<std::string>> allow_actions;
        std::string description;
        std::string create_time;
        std::string update_time;
    };

    class DbOperationLog final : public MongoDocument {
    public:
        bsoncxx::document::view get_document() override;

        std::string id;
        std::string user_id;
        std::string action;
        std::string bucket_name;
        std::string object_name;
        int32_t timestamp;
        std::string previous_state;
        std::string current_state;
        std::string description;
    };
} // namespace schema

#endif // STORAGE_DELIGHT_DB_SCHEMA_HPP
