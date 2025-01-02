//
// Created by cvrain on 24-9-22.
// 单例模式用户管理类
//

#ifndef USER_SERVICE_HPP
#define USER_SERVICE_HPP

#include "models/db_schema.hpp"
#include "mongo_service.hpp"
#include "type.hpp"
#include "utils/singleton_prototype.hpp"


namespace service_delight {

    class UserService : public util_delight::Singleton<UserService> {
    public:
        void init();
        auto add_one(const bsoncxx::document::value &user) -> schema::result<bsoncxx::oid, std::string>;
        auto add_one_v2(const bsoncxx::document::value &user_document)
                -> schema::result<bsoncxx::document::value, std::string>;
        auto get_by_id(const bsoncxx::oid &id) -> schema::result<bsoncxx::document::value, std::string>;
        auto get_by_name(const std::string &user_name) -> schema::result<bsoncxx::document::value, std::string>;
        auto is_exist(const std::string &user_name) -> schema::result<bool, std::string>;
        auto is_exist(const bsoncxx::oid &id) -> schema::result<bool, std::string>;
        bool admin_is_exist();
        bool is_admin(const bsoncxx::oid &id);

        auto list_ids() -> schema::result<std::vector<bsoncxx::oid>, std::string>;
        auto list_users() -> schema::result<std::vector<bsoncxx::document::value>, std::string>;
        auto remove_one(bsoncxx::oid) -> schema::result<bool, std::string>;

    private:
        mongocxx::collection user_collection;
    };

}  // namespace service_delight

#endif  //USER_SERVICE_HPP
