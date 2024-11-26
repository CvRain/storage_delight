//
// Created by cvrain on 24-10-2.
//

#include "db_schema.hpp"

#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

#include "schema_key.hpp"

using namespace schema;
using namespace bsoncxx::builder::basic;

bsoncxx::document::value DbUser::get_document() {
    auto document = make_document(kvp(key::bson_id, bsoncxx::oid{id}),
                                  kvp(key::name, name),
                                  kvp(key::password, password),
                                  kvp(key::group_id, group_id),
                                  kvp(key::user_role, role),
                                  kvp(key::create_time, create_time),
                                  kvp(key::update_time, update_time));
    return std::move(document);
}

nlohmann::json DbUser::to_json(const bsoncxx::document::value& document) {
    return nlohmann::json{{key::bson_id, document.view()[key::bson_id].get_oid().value.to_string()},
                          {key::name, document.view()[key::name].get_string()},
                          {key::password, document.view()[key::password].get_string()},
                          {key::user_role, document.view()[key::user_role].get_int32().value},
                          {key::group_id, document.view()[key::group_id].get_oid().value.to_string()},
                          {key::create_time, document.view()[key::create_time].get_int32().value},
                          {key::update_time, document.view()[key::update_time].get_int32().value}};
}

nlohmann::json DbUser::to_json() {
    return nlohmann::json{{key::bson_id, id.to_string()},
                          {key::name, name},
                          {key::password, password},
                          {key::user_role, role},
                          {key::group_id, group_id.to_string()},
                          {key::create_time, create_time},
                          {key::update_time, update_time}};
}

bsoncxx::document::value DbDataSource::get_document() {
    auto document = make_document(kvp(key::bson_id, id),
                                  kvp(key::name, name),
                                  kvp(key::url, url),
                                  kvp(key::access_key, access_key),
                                  kvp(key::secret_key, secret_key),
                                  kvp(key::create_time, create_time),
                                  kvp(key::is_https, is_https));
    return std::move(document);
}

nlohmann::json DbDataSource::to_json() {
    return nlohmann::json{{key::bson_id, id.to_string()},
                          {key::name, name},
                          {key::url, url},
                          {key::access_key, access_key},
                          {key::secret_key, secret_key},
                          {key::create_time, create_time},
                          {key::is_https, is_https}};
}

DbDataSource DbDataSource::from_bson(const bsoncxx::document::value& value) {
    DbDataSource dataSource;
    dataSource.id          = value.view()[key::bson_id].get_oid().value;
    dataSource.name        = value.view()[key::name].get_string();
    dataSource.url         = value.view()[key::url].get_string();
    dataSource.access_key  = value.view()[key::access_key].get_string();
    dataSource.secret_key  = value.view()[key::secret_key].get_string();
    dataSource.create_time = value.view()[key::create_time].get_int32();
    dataSource.is_https    = value.view()[key::is_https].get_bool();
    return dataSource;
}

bsoncxx::document::value DbGroup::get_document() {
    bsoncxx::builder::basic::array buckets_builder;
    for (const auto& [source_id, bucket_name]: buckets) {
        auto one_bucket = make_document(kvp(key::source_id, source_id), kvp(key::bucket_name, bucket_name));
        buckets_builder.append(one_bucket.view());
    }

    auto document = make_document(kvp(key::bson_id, id),
                                  kvp(key::name, name),
                                  kvp(key::owner_id, owner_id),
                                  kvp(key::members_id, util_delight::make_bson_array(members_id)),
                                  kvp(key::buckets, buckets_builder.view()),
                                  kvp(key::create_time, create_time),
                                  kvp(key::update_time, update_time));
    return std::move(document);
}

DbGroup DbGroup::from_bson(const bsoncxx::document::value& value) {
    DbGroup group;
    group.id       = value.view()[key::bson_id].get_oid().value;
    group.name     = value.view()[key::name].get_string();
    group.owner_id = value.view()[key::owner_id].get_oid().value;

    for (const auto  members_id_value = value.view()[key::members_id].get_array().value;
         const auto& it: members_id_value)
    {
        group.members_id.emplace_back(it.get_oid().value);
    }

    for (const auto buckets_document = value.view()[key::buckets].get_array().value; const auto& it: buckets_document) {
        const auto source_id   = it.get_document().view()[key::source_id].get_oid().value;
        const auto bucket_name = it.get_document().view()[key::bucket_name].get_string().value;
        group.buckets.emplace(source_id, bucket_name);
    }

    group.create_time = value.view()[key::create_time].get_int32();
    group.update_time = value.view()[key::update_time].get_int32();
    return group;
}

nlohmann::json DbGroup::to_json() {
    nlohmann::json members_json = nlohmann::json::array();
    for (const auto& it: members_id) {
        members_json.emplace_back(it.to_string());
    }

    nlohmann::json buckets_array = nlohmann::json::array();
    for (const auto& [source_id, bucket_name]: buckets) {
        auto one_bucket = nlohmann::json{{key::source_id, source_id.to_string()},
                                          {key::bucket_name, bucket_name}};
        buckets_array.emplace_back(one_bucket);
    }

    return nlohmann::json{{key::bson_id, id.to_string()},
                          {key::name, name},
                          {key::owner_id, owner_id.to_string()},
                          {key::members_id, members_json},
                          {key::buckets, buckets_array},
                          {key::create_time, create_time},
                          {key::update_time, update_time}};
}

bsoncxx::document::value DbPermission::get_document() {
    auto permission_document =
            make_document(kvp(key::permission::allow_read,
                              util_delight::make_bson_array(allow_actions.at(key::permission::allow_read))),
                          kvp(key::permission::allow_write,
                              util_delight::make_bson_array(allow_actions.at(key::permission::allow_write))),
                          kvp(key::permission::allow_delete,
                              util_delight::make_bson_array(allow_actions.at(key::permission::allow_delete))),
                          kvp(key::permission::allow_grant,
                              util_delight::make_bson_array(allow_actions.at(key::permission::allow_grant))),
                          kvp(key::permission::allow_revoke,
                              util_delight::make_bson_array(allow_actions.at(key::permission::allow_revoke))),
                          kvp(key::permission::allow_create_bucket,
                              util_delight::make_bson_array(allow_actions.at(key::permission::allow_create_bucket))),
                          kvp(key::permission::allow_delete_bucket,
                              util_delight::make_bson_array(allow_actions.at(key::permission::allow_delete_bucket))),
                          kvp(key::permission::allow_update_bucket,
                              util_delight::make_bson_array(allow_actions.at(key::permission::allow_update_bucket))),
                          kvp(key::permission::allow_list,
                              util_delight::make_bson_array(allow_actions.at(key::permission::allow_list))));

    auto one_bucket = make_document(kvp(key::source_id, bucket.first), kvp(key::bucket_name, bucket.second));

    auto document = make_document(kvp(key::bson_id, bsoncxx::oid{id}),
                                  kvp(key::buckets, std::move(one_bucket)),
                                  kvp(key::allow_actions, std::move(permission_document)));

    return std::move(document);
}

nlohmann::json DbPermission::to_json() {
    nlohmann::json allow_actions_json{};
    for (const auto& [fst, snd]: allow_actions) {
        for (const auto& it: snd) {
            allow_actions_json[fst].emplace_back(it.to_string());
        }
    }

    nlohmann::json one_bucket{
        {key::source_id, bucket.first.to_string()},
        {key::bucket_name, bucket.second}
    };

    return nlohmann::json{{key::bson_id, id.to_string()},
                          {key::buckets, one_bucket},
                          {key::allow_actions, allow_actions_json},
                          {key::create_time, create_time},
                          {key::update_time, update_time}};
}

bsoncxx::document::value DbOperationLog::get_document() {
    auto document = make_document(kvp(key::bson_id, id),
                                  kvp(key::user_id, user_id),
                                  kvp(key::bucket_name, bucket_name),
                                  kvp(key::object_name, object_name),
                                  kvp(key::source_name, source_id),
                                  kvp(key::timestamp, timestamp),
                                  kvp(key::action, action),
                                  kvp(key::current_state, current_state),
                                  kvp(key::previous_state, previous_state),
                                  kvp(key::description, description),
                                  kvp(key::request_ip, request_ip));
    return std::move(document);
}
nlohmann::json DbOperationLog::to_json() {
    return nlohmann::json{{key::bson_id, id.to_string()},
                          {key::user_id, user_id.to_string()},
                          {key::bucket_name, bucket_name},
                          {key::object_name, object_name},
                          {key::source_name, source_id.to_string()},
                          {key::timestamp, timestamp},
                          {key::action, action},
                          {key::current_state, current_state},
                          {key::previous_state, previous_state},
                          {key::description, description},
                          {key::request_ip, request_ip}};
}
