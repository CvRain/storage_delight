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

        /**
         * 添加用户组
         * @param group 用户组信息
         * @return 返回插入信息，以及错误原因
         */
        auto add_group(schema::DbGroup *group) -> schema::result<std::string, std::string>;

        /**
         * 获得用户组信息
         * @param group_id 用户组id
         * @return DbGroup类型，异常返回std::nullopt和错误信息
         */
        auto get_group(const bsoncxx::oid &group_id) -> schema::result<schema::DbGroup, std::string>;

        /**
         * 获得用户组信息
         * @param group_id 用户组id
         * @return bsoncxx::document::value类型，异常返回std::nullopt和错误信息
         */
        auto get_group_to_bson(const bsoncxx::oid &group_id) -> schema::result<bsoncxx::document::value, std::string>;

        /**
         * 检查用户组是否存在
         * @param group_id 用户组id
         * @return 如果存在此用户组则返回true否则返回false,异常返回std::nullopt和错误信息
         */
        auto group_exist(const bsoncxx::oid &group_id) -> schema::result<bool, std::string>;

        /**
         * 更新用户组信息
         * @param group 用户组信息
         * @return 返回插入信息，以及错误原因
         */
        auto update_group(schema::DbGroup* group) -> schema::result<bool, std::string>;

        /**
         * 添加成员至用户组
         * @param group_id 需要插入的用户工作组oid
         * @param member_id 需要插入的用户id
         * @return 返回插入信息，以及错误原因
         */
        auto add_member(const bsoncxx::oid &group_id, const bsoncxx::oid &member_id) -> schema::result<bool, std::string>;

        /**
         * 批量插入成员至用户组
         * @param group_id 需要插入的用户工作组oid
         * @param members_id  需要插入的用户id
         * @return 返回插入信息，以及错误原因
         */
        auto add_members(const bsoncxx::oid &group_id, const std::vector<bsoncxx::oid> &members_id) -> schema::result<bool, std::string>;

        auto remove_member(const bsoncxx::oid &group_id, const bsoncxx::oid &member_id) -> schema::result<bool, std::string>;

        auto get_members(const bsoncxx::oid &group_id) -> schema::result<std::vector<bsoncxx::oid>, std::string>;

        auto rename(const bsoncxx::oid &group_id, const std::string &new_name) -> schema::result<bool, std::string>;

        auto is_member(const bsoncxx::oid &group_id, const bsoncxx::oid &member_id) -> schema::result<bool, std::string>;

        auto get_all_groups() -> schema::result<std::vector<bsoncxx::document::value>, std::string>;

        auto delete_group(const bsoncxx::oid &group_id) -> schema::result<bool, std::string>;

        auto add_bucket(const bsoncxx::oid &group_id, const bsoncxx::oid &bucket_id) -> schema::result<bool, std::string>;

        auto remove_bucket(const bsoncxx::oid &group_id, const bsoncxx::oid &bucket_id) -> schema::result<bool, std::string>;

        auto get_bucket_ids(const bsoncxx::oid &group_id) -> schema::result<std::vector<bsoncxx::oid>, std::string>;

        auto is_bucket_exist(const bsoncxx::oid &group_id, const bsoncxx::oid &bucket_id)
                -> schema::result<bool, std::string>;

        /**
         * 获得工作组所有者id
         * @param group_id 用户组id
         * @return 如果存在此工作组则返回id否则返回<code>std::nullopt</code>和错误信息
         */
        auto get_group_owner(const bsoncxx::oid &group_id) -> schema::result<bsoncxx::oid, std::string>;

    private:
        mongocxx::collection group_collection;
    };

} // namespace service_delight