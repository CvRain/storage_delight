#pragma once

#include <drogon/HttpController.h>
#include "models/user_request.hpp"
#include "models/db_schema.hpp"
#include "filters/user_filter.hpp"

using namespace drogon;

namespace api {
    class User final : public drogon::HttpController<User> {
    public:
        METHOD_LIST_BEGIN
            METHOD_ADD(User::add_user, "/add", Post);
			METHOD_ADD(User::login, "/login", Post);
            METHOD_ADD(User::get_user_by_id, "/id/{user_id}", Get, middleware::LoginMiddlewareName);

        METHOD_LIST_END

        static void add_user(const HttpRequestPtr &req,
                             std::function<void(const HttpResponsePtr &)> &&callback,
                             schema::BaseUser &&user_request);

        static void
        login(const HttpRequestPtr &req,
              std::function<void(const HttpResponsePtr &)> &&callback,
              schema::BaseUser &&user_request);

        static void
        get_user_by_id(const HttpRequestPtr &req,
                 std::function<void(const HttpResponsePtr &)> &&callback);
    };
}
