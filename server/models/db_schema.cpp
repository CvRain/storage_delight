//
// Created by cvrain on 24-10-2.
//

#include "db_schema.hpp"

#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/stream/document.hpp>

#include "schema_key.hpp"
#include "utils/date.h"

using namespace schema;
using namespace bsoncxx::builder::basic;

bsoncxx::document::value DbUser::get_document() {
    auto document = make_document(kvp(key::bson_id, bsoncxx::oid{id}), kvp(key::name, name),
                                  kvp(key::password, password), kvp(key::group_id, group_id), kvp(key::user_role, role),
                                  kvp(key::create_time, create_time), kvp(key::update_time, update_time));
    return std::move(document);
}

nlohmann::json DbUser::to_json(const bsoncxx::document::value &document) {
    return nlohmann::json{{key::bson_id, document.view()[key::bson_id].get_oid().value.to_string()},
                          {key::name, document.view()[key::name].get_string()},
                          {key::password, document.view()[key::password].get_string()},
                          {key::user_role, document.view()[key::user_role].get_int32().value},
                          {key::group_id, document.view()[key::group_id].get_oid().value.to_string()},
                          {key::create_time, document.view()[key::create_time].get_int32().value},
                          {key::update_time, document.view()[key::update_time].get_int32().value}};
}

bsoncxx::document::value DbBucket::get_document() {
    auto document =
            make_document(kvp(key::bson_id, bsoncxx::oid{id}), kvp(key::data_source, data_source),
                          kvp(key::bucket_name, bucket_name), kvp(key::group_id, group_id),
                          kvp(key::permission_id, permission_id), kvp(key::tags, util_delight::make_bson_array(tags)),
                          kvp(key::create_time, create_time), kvp(key::update_time, update_time));
    return std::move(document);
}

bsoncxx::document::value DbDataSource::get_document() {
    auto document = make_document(kvp(key::bson_id, bsoncxx::oid{id}), kvp(key::name, name), kvp(key::url, url),
                                  kvp(key::access_key, access_key), kvp(key::secret_key, secret_key),
                                  kvp(key::create_time, create_time));
    return std::move(document);
}

bsoncxx::document::value DbGroup::get_document() {
    auto document = make_document(kvp(key::bson_id, id), kvp(key::name, name), kvp(key::owner_id, owner_id),
                                  kvp(key::members_id, util_delight::make_bson_array(members_id)),
                                  kvp(key::bucket_group_id, util_delight::make_bson_array(bucket_group_id)),
                                  kvp(key::create_time, create_time), kvp(key::update_time, update_time));
    return std::move(document);
}

DbGroup DbGroup::from_bson(const bsoncxx::document::value &value) {
    DbGroup group;
    group.id = value.view()[key::bson_id].get_oid().value;
    group.name = value.view()[key::name].get_string();
    group.owner_id = value.view()[key::owner_id].get_oid().value;

    for (const auto members_id_value = value.view()[key::members_id].get_array().value;
         const auto& it : members_id_value) {
        group.members_id.emplace_back(it.get_oid().value);
    }

    for(const auto bucket_group_id_value = value.view()[key::bucket_group_id].get_array().value;
        const auto& it : bucket_group_id_value) {
        group.bucket_group_id.emplace_back(it.get_oid().value);
    }

    group.create_time = value.view()[key::create_time].get_int32();
    group.update_time = value.view()[key::update_time].get_int32();
    return group;
}
nlohmann::json DbGroup::to_json() {
    nlohmann::json members_json = nlohmann::json::array();
    for(const auto& it : members_id) {
        members_json.emplace_back(it.to_string());
    }

    nlohmann::json bucket_group_json = nlohmann::json::array();
    for(const auto& it : bucket_group_id) {
        bucket_group_json.emplace_back(it.to_string());
    }

    return nlohmann::json{{key::bson_id, id.to_string()},
                          {key::name, name},
                          {key::owner_id, owner_id.to_string()},
                          {key::members_id, members_json},
                          {key::bucket_group_id, bucket_group_json},
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

    auto document =
            make_document(
                kvp(key::bson_id, bsoncxx::oid{id}),
                kvp(key::name, name),
                kvp(key::description, description),
                kvp(key::bucket_id, bucket_id),
                kvp(key::allow_actions, std::move(permission_document))
                );

    return std::move(document);
}

bsoncxx::document::value DbOperationLog::get_document() {
    auto document =
            make_document(kvp(key::bson_id, id), kvp(key::user_id, user_id), kvp(key::bucket_name, bucket_name),
                          kvp(key::object_name, object_name), kvp(key::timestamp, timestamp), kvp(key::action, action),
                          kvp(key::current_state, current_state), kvp(key::previous_state, previous_state),
                          kvp(key::description, description), kvp(key::request_ip, request_ip));
    return std::move(document);
}
