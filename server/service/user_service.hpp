//
// Created by cvrain on 24-9-22.
// 单例模式用户管理类
//

#ifndef USER_SERVICE_HPP
#define USER_SERVICE_HPP

#include "models/db_schema.hpp"
#include "mongo_service.hpp"
#include "utils/singleton_prototype.hpp"

namespace service_delight {

    class UserService : public util_delight::Singleton<UserService> {
    public:
        void init();
        schema::User add_user(const schema::BaseUser &user);
        bool user_is_exist(const std::string &user_name);
        bool admin_is_exist();
        std::optional<schema::User> get_user_by_name(const std::string &user_name);
        std::optional<schema::User> get_user_by_id(const std::string &id);
        std::optional<schema::User> remove_user(const std::string &user_name);

    private:
        mongocxx::collection user_collection;
    };

} // namespace service_delight

#endif //USER_SERVICE_HPP
