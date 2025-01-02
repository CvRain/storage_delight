#pragma once

#include <drogon/HttpController.h>

#include "basic_value.hpp"
#include "filters/user_filter.hpp"
#include "nlohmann_json_request.hpp"

using namespace drogon;

namespace api {
    class User final : public HttpController<User> {
    public:
        METHOD_LIST_BEGIN
        METHOD_ADD(User::add_user, "/add", Options, Post);
        METHOD_ADD(User::login, "/login", Options, Post);
        METHOD_ADD(User::get_user_by_id, "/id/{user_id}", Options, Get);
        METHOD_ADD(User::user_info, "/info", Options, Get, model_delight::basic_value::middleware::UserExist);
        METHOD_ADD(User::all_user_info, "/info/all", Options, Get);
        METHOD_ADD(User::remove_user, "/remove", Options, Delete, model_delight::basic_value::middleware::UserExist);
        METHOD_ADD(User::get_user_name, "/name", Options, Get);
        METHOD_ADD(User::is_admin_account, "/is_admin", Options, Get, model_delight::basic_value::middleware::UserExist);

        METHOD_LIST_END

        static void add_user(model_delight::NlohmannJsonRequestPtr        &&req,
                             std::function<void(const HttpResponsePtr &)> &&callback);

        static void login(model_delight::NlohmannJsonRequestPtr        &&req,
                          std::function<void(const HttpResponsePtr &)> &&callback);

        static void get_user_by_id(model_delight::NlohmannJsonRequestPtr        &&req,
                                   std::function<void(const HttpResponsePtr &)> &&callback);

        static void user_info(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);

        static void all_user_info(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);

        static void remove_user(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);

        static void get_user_name(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);

        static void is_admin_account(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
    };
}  // namespace api
