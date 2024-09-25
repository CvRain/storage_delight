#pragma once

#include <drogon/HttpController.h>

#include "filters/user_filter.hpp"
#include "models/basic_value.hpp"
#include "models/db_schema.hpp"
#include "models/user_request.hpp"
#include "nlohmann_json_request.hpp"

using namespace drogon;

namespace api {
    class User final : public drogon::HttpController<User> {
    public:
        METHOD_LIST_BEGIN
        METHOD_ADD(User::add_user, "/add", Post);
        METHOD_ADD(User::login, "/login", Post);
        METHOD_ADD(User::get_user_by_id, "/id/{user_id}", Get, model_delight::middleware::LoginAuth);

        METHOD_LIST_END

        static void add_user(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback,
                             schema::BaseUser &&user_request);

        static void login(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback,
                          schema::BaseUser &&user_request);

        static void get_user_by_id(model_delight::NlohmannJsonRequestPtr &&req, std::function<void(const HttpResponsePtr &)> &&callback);
    };
} // namespace api
