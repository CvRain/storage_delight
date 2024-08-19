//
// Created by cvrain-thinkbook on 24-8-18.
//

#include "user_request.hpp"
#include <spdlog/spdlog.h>

namespace drogon {
    template<>
    schema::BaseUser fromRequest(const HttpRequest &request) {
        const auto &json = request.getJsonObject();

        spdlog::warn("{}", schema::BaseUser{}.user_name);

        if (!json) {
            spdlog::warn("schema::CommonUser::fromRequest: request body is empty");
            return {};
        }

        //判断json是否为空
        if (json->empty()) {
            spdlog::warn("schema::CommonUser::fromRequest: json value is empty");
            return {};
        }

        //检查json是否包含user_name,user_password,role
        if (!json->isMember("user_name")
            || !json->isMember("user_password")
            || !json->isMember("role")) {
            spdlog::warn("schema::CommonUser::fromRequest: failed to parse json");
            return {};
        }

        if (!json) {
            spdlog::error("Failed to parse json: schema::CommonUser");
            return {};
        }
        return schema::BaseUser{
                .role = static_cast<schema::UserRole>((*json)["role"].asInt()),
                .user_name =  (*json)["user_name"].asString(),
                .password = (*json)["password"].asString(),
        };
    }
}