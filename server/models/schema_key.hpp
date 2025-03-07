//
// Created by cvrain on 24-9-23.
//

#ifndef SCHEMA_KEY_HPP
#define SCHEMA_KEY_HPP

namespace schema::key {
    const std::string common_id = "id";
    const std::string bson_id = "_id";
    const std::string oid = "$oid";
    const std::string name = "name";
    const std::string user_id = "user_id";
    const std::string password = "password";
    const std::string group_id = "group_id";
    const std::string user_role = "role";
    const std::string create_time = "create_time";
    const std::string update_time = "update_time";
    const std::string data_source = "data_source";
    const std::string bucket_name = "bucket_name";
    const std::string source_name = "source_name";
    const std::string permission_id = "permission_id";
    const std::string tags = "tags";
    const std::string access_key = "access_key";
    const std::string secret_key = "secret_key";
    const std::string owner_id = "owner_id";
    const std::string members_id = "members_id";
    const std::string bucket_group_id = "bucket_group_id";
    const std::string buckets = "buckets";
    const std::string allow_actions = "allow_actions";
    const std::string action = "action";
    const std::string description = "description";
    const std::string object_name = "object_name";
    const std::string timestamp = "timestamp";
    const std::string previous_state = "previous_state";
    const std::string current_state = "current_state";
    const std::string request_ip = "request_ip";
    const std::string url = "url";
    const std::string bucket_id = "bucket_id";
    const std::string token = "token";
    const std::string is_https = "is_https";
    const std::string source_id = "source_id";

    namespace permission {
        const std::string allow_read = "allow_read";
        const std::string allow_write = "allow_write";
        const std::string allow_delete = "allow_delete";
        const std::string allow_grant = "allow_grant";
        const std::string allow_revoke = "allow_revoke";
        const std::string allow_list = "allow_list";
        const std::string allow_create_bucket = "allow_create_bucket";
        const std::string allow_delete_bucket = "allow_delete_bucket";
        const std::string allow_update_bucket = "allow_update_bucket";
    }

    namespace database{
        const std::string db_name = "storage_delight";
    }

    namespace collection {
        constexpr std::string_view user = "user";
        constexpr std::string_view group = "group";
        constexpr std::string_view operation_logs = "operation_logs";
        constexpr std::string_view data_source = "data_source";
        constexpr std::string_view bucket = "bucket";
        constexpr std::string_view permission = "permission";
    }
}

#endif //SCHEMA_KEY_HPP
