#include "user_response.hpp"
#include <algorithm>

namespace model_delight {
    UserResponse& UserResponse::set_users(std::vector<schema::User>&&source_users) {
        this->users = std::move(source_users);
        return *this;
    }

    UserResponse& UserResponse::add_user(const schema::User&user) {
        this->users.emplace_back(user);
        return *this;
    }

    nlohmann::json UserResponse::to_json() {
        auto json = HttpResponse::to_json();
        std::ranges::for_each(users.begin(), users.end(), [&](const schema::User&user) {
            // Json::Value user_json;
            // user_json["id"] = user.id;
            // user_json["role"] = user.role;
            // user_json["user_name"] = user.user_name;
            // user_json["password"] = user.password;
            // user_json["create_time"] = user.create_time;
            // json["users"].append(user_json);
            json["users"].push_back(nlohmann::json{
                {"id", user.id},
                {"role", user.role},
                {"user_name", user.user_name},
                {"password", user.password},
                {"create_time", user.create_time}
            });
        });
        return json;
    }
}
