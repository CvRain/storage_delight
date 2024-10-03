//
// Created by cvrain on 24-10-2.
//

#include "db_schema.hpp"

#include <bsoncxx/builder/basic/array.hpp>

#include "schema_key.hpp"
#include "utils/date.h"

using namespace schema;
using namespace bsoncxx::builder::basic;

bsoncxx::document::view DbUser::get_document() {
    const auto document = make_document(
            kvp(key::bson_id, bsoncxx::oid{id}),
            kvp(key::name, name),
            kvp(key::password, password),
            kvp(key::group_id, group_id),
            kvp(key::user_role, role),
            kvp(key::create_time, create_time),
            kvp(key::update_time, update_time)
            );
    return document.view();
}

bsoncxx::document::view DbBucket::get_document() {
    const auto document = make_document(
            kvp(key::bson_id, bsoncxx::oid{id}),
            kvp(key::data_source, data_source),
            kvp(key::bucket_name, bucket_name),
            kvp(key::group_id, group_id),
            kvp(key::permission_id, permission_id),
            kvp(key::tags, make_array(tags)),
            kvp(key::create_time, create_time),
            kvp(key::update_time, update_time)
            );
    return document.view();
}

bsoncxx::document::view DbDataSource::get_document() {
    const auto document = make_document(
            kvp(key::bson_id, bsoncxx::oid{id}),
            kvp(key::name, name),
            kvp(key::url, url),
            kvp(key::access_key, access_key),
            kvp(key::secret_key, secret_key),
            kvp(key::create_time, create_time)
            );
    return document.view();
}

bsoncxx::document::view DbGroup::get_document() {
    const auto document = make_document(
            kvp(key::bson_id, bsoncxx::oid{id}),
            kvp(key::name, name),
            kvp(key::owner_id, owner_id),
            kvp(key::members_id, make_array(members_id)),
            kvp(key::bucket_group_id, make_array(bucket_group_id)),
            kvp(key::create_time, create_time),
            kvp(key::update_time, update_time)
            );
    return document.view();
}

bsoncxx::document::view DbPermission::get_document() {
    const auto permission_document = make_document(
            kvp(key::permission::allow_read,
                make_array(allow_actions.at(key::permission::allow_read))),
            kvp(key::permission::allow_write,
                make_array(allow_actions.at(key::permission::allow_write))),
            kvp(key::permission::allow_delete,
                make_array(allow_actions.at(key::permission::allow_delete))),
            kvp(key::permission::allow_grant,
                make_array(allow_actions.at(key::permission::allow_grant))),
            kvp(key::permission::allow_revoke,
                make_array(allow_actions.at(key::permission::allow_revoke))),
            kvp(key::permission::allow_create_bucket,
                make_array(allow_actions.at(key::permission::allow_create_bucket))),
            kvp(key::permission::allow_delete_bucket,
                make_array(allow_actions.at(key::permission::allow_delete_bucket))),
            kvp(key::permission::allow_update_bucket,
                make_array(allow_actions.at(key::permission::allow_update_bucket))),
            kvp(key::permission::allow_list,
                make_array(allow_actions.at(key::permission::allow_list)))
            );

    const auto document = make_document(
            kvp(key::bson_id, bsoncxx::oid{id}),
            kvp(key::name, name),
            kvp(key::description, description),
            kvp(key::bucket_id, bucket_id),
            kvp(key::allow_actions, permission_document)
            );

    return document.view();
}

bsoncxx::document::view DbOperationLog::get_document() {
    const auto document = make_document(
            kvp(key::bson_id, bsoncxx::oid{id}),
            kvp(key::user_id, user_id),
            kvp(key::bucket_name, bucket_name),
            kvp(key::object_name, object_name),
            kvp(key::timestamp, timestamp),
            kvp(key::action, action),
            kvp(key::current_state, current_state),
            kvp(key::previous_state, previous_state),
            kvp(key::description, description)
    );
    return document.view();
}
