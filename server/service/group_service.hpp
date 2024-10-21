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

        auto add_group(schema::DbGroup *group) -> schema::result<std::string, std::string>;

        auto get_group(const bsoncxx::oid &group_id) -> schema::result<schema::DbGroup, std::string>;

        auto group_exist(const bsoncxx::oid &group_id) -> schema::result<bool, std::string>;

        auto add_member(const bsoncxx::oid &group_id, const bsoncxx::oid &member_id) -> schema::result<bool, std::string>;

        /**
         * 批量插入成员至用户组
         * @param group_id 需要插入的用户工作组oid
         * @param member_ids  需要插入的用户id
         * @return 返回插入信息，以及错误原因
         */
        auto add_members(const bsoncxx::oid &group_id, const std::vector<bsoncxx::oid> &member_ids) -> schema::result<bool, std::string>;

        auto remove_member(const bsoncxx::oid &group_id, const bsoncxx::oid &member_id) -> schema::result<bool, std::string>;

        auto get_members(const bsoncxx::oid &group_id) -> schema::result<std::vector<bsoncxx::oid>, std::string>; // todo

        auto rename(const bsoncxx::oid &group_id, const std::string &new_name) -> schema::result<bool, std::string>; // todo

        auto is_member(const bsoncxx::oid &group_id, const bsoncxx::oid &member_id) -> schema::result<bool, std::string>; // todo

        auto get_all_groups() -> schema::result<std::vector<bsoncxx::document::value>, std::string>; // todo

        auto delete_group(const bsoncxx::oid &group_id) -> schema::result<bool, std::string>; // todo

        auto add_bucket(const bsoncxx::oid &group_id, const bsoncxx::oid &bucket_id) -> schema::result<bool, std::string>; // todo

        auto remove_bucket(const bsoncxx::oid &group_id, const bsoncxx::oid &bucket_id) -> schema::result<bool, std::string>; // todo

        auto get_bucket_ids(const bsoncxx::oid &group_id) -> schema::result<std::vector<bsoncxx::oid>, std::string>; // todo

        auto is_bucket_exist(const bsoncxx::oid &group_id, const bsoncxx::oid &bucket_id) -> schema::result<bool, std::string>; // todo

    private:
        mongocxx::collection group_collection;
    };

} // namespace service_delight