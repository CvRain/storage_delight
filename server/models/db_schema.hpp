//
// Created by cvrain on 24-8-13.
//

#ifndef STORAGE_DELIGHT_DB_SCHEMA_HPP
#define STORAGE_DELIGHT_DB_SCHEMA_HPP

#include <bsoncxx/builder/basic/document.hpp>
#include <map>
#include <string>
#include <vector>

#include "utils/date.h"
#include "utils/format.h"

namespace schema {
    enum UserRole : int { TypeNone = -1, TypeAdmin = 0, TypeUser = 1, TypeRoleMax };

    class MongoDocument {
    public:
        MongoDocument() {
            id = bsoncxx::oid{};
            create_time = util_delight::Date::get_current_timestamp_32();
        };
        virtual ~MongoDocument() = default;
        virtual bsoncxx::document::value get_document() = 0;
        // virtual nlohmann::json to_json() = 0;

    public:
        bsoncxx::oid id{};
        int32_t create_time{};
    };

    class DbUser final : public MongoDocument {
    public:
        bsoncxx::document::value get_document() override;
        static nlohmann::json to_json(const bsoncxx::document::value& document);

        std::string name;
        std::string password;
        bsoncxx::oid group_id;
        int32_t role = UserRole::TypeUser;
        int32_t update_time;
    };

    class DbBucket final : public MongoDocument {
    public:
        bsoncxx::document::value get_document() override;

        std::string data_source;
        std::string bucket_name;
        bsoncxx::oid group_id;
        bsoncxx::oid permission_id;
        std::vector<std::string> tags;
        int32_t update_time;
    };

    class DbDataSource final : public MongoDocument {
    public:
        bsoncxx::document::value get_document() override;

        std::string id;
        std::string name;
        std::string url;
        std::string access_key;
        std::string secret_key;
        std::string create_time;
    };

    class DbGroup final : public MongoDocument {
    public:
        bsoncxx::document::value get_document() override;
        static DbGroup from_bson(const bsoncxx::document::value &value);
        nlohmann::json to_json();

        bsoncxx::oid id;
        std::string name;
        bsoncxx::oid owner_id;
        std::vector<bsoncxx::oid> members_id;
        std::vector<bsoncxx::oid> bucket_group_id;
        int32_t create_time;
        int32_t update_time;
    };

    /**
     * @param allow_actions: key: action name, value: user id
     * e.g. "allow_read":["0001","0002","0003"]
     */
    class DbPermission final : public MongoDocument {
    public:
        bsoncxx::document::value get_document() override;

        bsoncxx::oid bucket_id;
        std::string name;
        std::map<std::string, std::vector<std::string>> allow_actions;
        std::string description;
        std::string create_time;
        std::string update_time;
    };

    class DbOperationLog final : public MongoDocument {
    public:
        bsoncxx::document::value get_document() override;

        bsoncxx::oid user_id{};
        std::string action{};
        std::string bucket_name{};
        std::string object_name{};
        int32_t timestamp = util_delight::Date::get_current_timestamp_32();
        std::string previous_state{};
        std::string current_state{};
        std::string description{};
        std::string request_ip{};
    };
} // namespace schema

#endif // STORAGE_DELIGHT_DB_SCHEMA_HPP
