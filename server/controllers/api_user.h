#pragma once

#include <drogon/HttpController.h>
#include "models/user_request.hpp"
#include "models/db_schema.hpp"

using namespace drogon;

namespace api {
    class User final : public drogon::HttpController<User> {
    public:
        METHOD_LIST_BEGIN
            METHOD_ADD(User::add_user, "/add", Post);
			METHOD_ADD(User::login, "/login", Post);

        METHOD_LIST_END

        static void add_user(const HttpRequestPtr &req,
                             std::function<void(const HttpResponsePtr &)> &&callback,
                             schema::BaseUser &&user_request);

        static void
        login(const HttpRequestPtr &req,
              std::function<void(const HttpResponsePtr &)> &&callback,
              schema::BaseUser &&user_request);
    };
}
