#pragma once

#include <drogon/HttpController.h>

#include "nlohmann_json_request.hpp"

using namespace drogon;

namespace api {
    class User final : public HttpController<User> {
    public:
        METHOD_LIST_BEGIN
        METHOD_ADD(User::add_user, "/add", Options, Post);
        METHOD_ADD(User::login, "/login", Options, Post);
        METHOD_ADD(User::get_user_by_id, "/id/{user_id}", Options, Get, "drogon::middleware::LoginMiddleware");

        METHOD_LIST_END

        static void add_user(model_delight::NlohmannJsonRequestPtr &&req,
                             std::function<void(const HttpResponsePtr &)> &&callback);

        static void login(model_delight::NlohmannJsonRequestPtr &&req,
                          std::function<void(const HttpResponsePtr &)> &&callback);

        static void get_user_by_id(model_delight::NlohmannJsonRequestPtr &&req,
                                   std::function<void(const HttpResponsePtr &)> &&callback);
    };
} // namespace api
