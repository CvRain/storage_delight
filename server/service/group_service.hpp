//
// Created by cvrain on 24-10-5.
//

#pragma once

#include "models/db_schema.hpp"
#include "mongo_service.hpp"
#include "type.hpp"
#include "utils/singleton_prototype.hpp"

namespace service_delight {

    class GroupService : public util_delight::Singleton<GroupService> {
    public:
        void init();
        auto add_one(schema::DbGroup *group) -> schema::result<std::string, std::string>;

        auto add_one(const bsoncxx::document::value& group) -> schema::result<bsoncxx::oid, std::string>;

        auto get_one(const bsoncxx::oid &group_id) -> schema::result<schema::DbGroup, std::string>;

        auto get_one_to_bson(const bsoncxx::oid &group_id) -> schema::result<bsoncxx::document::value, std::string>;

        auto is_exist(const bsoncxx::oid &group_id) -> schema::result<bool, std::string>;

        auto update_one(schema::DbGroup *group) -> schema::result<bool, std::string>;

        auto add_member(const bsoncxx::oid &group_id, const bsoncxx::oid &member_id)
                -> schema::result<bool, std::string>;

        auto add_members(const bsoncxx::oid &group_id, const std::vector<bsoncxx::oid> &members_id)
                -> schema::result<bool, std::string>;

        auto remove_member(const bsoncxx::oid &group_id, const bsoncxx::oid &member_id)
                -> schema::result<bool, std::string>;

        auto get_members(const bsoncxx::oid &group_id) -> schema::result<std::vector<bsoncxx::oid>, std::string>;

        auto rename(const bsoncxx::oid &group_id, const std::string &new_name) -> schema::result<bool, std::string>;

        auto is_member(const bsoncxx::oid &group_id, const bsoncxx::oid &member_id)
                -> schema::result<bool, std::string>;

        auto get_all() -> schema::result<std::vector<bsoncxx::document::value>, std::string>;

        auto remove_one(const bsoncxx::oid &group_id) -> schema::result<bool, std::string>;

        auto add_bucket(const bsoncxx::oid &group_id, const std::string& bucket_name)
                -> schema::result<bool, std::string>;

        auto remove_bucket(const bsoncxx::oid &group_id, const std::string& bucket_name)
                -> schema::result<bool, std::string>;

        auto get_owner(const bsoncxx::oid &group_id) -> schema::result<bsoncxx::oid, std::string>;

    private:
        mongocxx::collection group_collection;
    };

}  // namespace service_delight
