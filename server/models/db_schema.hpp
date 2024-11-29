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
    enum class UserRole : int32_t { TypeNone = -1, TypeAdmin = 0, TypeUser = 1, TypeRoleMax };

    class MongoDocument
    {
    public:
        MongoDocument() {
            id = bsoncxx::oid{};
            create_time = util_delight::Date::get_current_timestamp_32();
        };
        virtual ~MongoDocument() = default;
        virtual bsoncxx::document::value get_document() = 0;
        virtual nlohmann::json to_json() = 0;

    public:
        bsoncxx::oid id{};
        int32_t create_time{};
    };

    class DbUser final : public MongoDocument
    {
    public:
        bsoncxx::document::value get_document() override;
        static nlohmann::json to_json(const bsoncxx::document::value& document);
        nlohmann::json to_json() override;

        std::string name;
        std::string password;
        bsoncxx::oid group_id;
        int32_t role = static_cast<int32_t>(UserRole::TypeUser);
        int32_t update_time;
    };

    class DbDataSource final : public MongoDocument
    {
    public:
        bsoncxx::document::value get_document() override;
        nlohmann::json to_json() override;
        static DbDataSource from_bson(const bsoncxx::document::value& value);

        bsoncxx::oid id;
        std::string name;
        std::string url;
        std::string access_key;
        std::string secret_key;
        int32_t create_time;
        bool is_https;
    };

    class DbGroup final : public MongoDocument
    {
    public:
        bsoncxx::document::value get_document() override;
        static DbGroup from_bson(const bsoncxx::document::value& value);
        nlohmann::json to_json() override;

        std::string name{};
        bsoncxx::oid owner_id{};
        std::vector<bsoncxx::oid> members_id{};
        std::vector<std::pair<bsoncxx::oid, std::string>> buckets{};  //桶： 包括存储源id,桶名称
        int32_t update_time;
    };

    /**
     * @param allow_actions: key: action name, value: user id
     * e.g. "allow_read":["0001","0002","0003"]
     */
    class DbPermission final : public MongoDocument
    {
    public:
        bsoncxx::document::value get_document() override;
        nlohmann::json to_json() override;

        std::pair<bsoncxx::oid, std::string> bucket;    //桶： 包括存储源id,桶名称
        std::map<std::string, std::vector<bsoncxx::oid>> allow_actions; //权限名称，用户id
        std::string create_time;
        std::string update_time;
    };

    class DbOperationLog final : public MongoDocument
    {
    public:
        bsoncxx::document::value get_document() override;
        nlohmann::json to_json() override;

        bsoncxx::oid user_id{};
        std::string action{};
        std::string bucket_name{};
        std::string object_name{};
        bsoncxx::oid source_id{};
        int32_t timestamp = util_delight::Date::get_current_timestamp_32();
        std::string previous_state{};
        std::string current_state{};
        std::string description{};
        std::string request_ip{};
    };
}   // namespace schema

#endif   // STORAGE_DELIGHT_DB_SCHEMA_HPP
