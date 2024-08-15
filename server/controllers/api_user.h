#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

namespace api {
    class User final : public drogon::HttpController<User> {
    public:
        METHOD_LIST_BEGIN
            METHOD_ADD(User::add_user, "/add", Post);
            METHOD_ADD(User::find_user_by_id, "/id", Get);
            METHOD_ADD(User::find_user_by_name, "/name", Get);

        METHOD_LIST_END

        static void add_user(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr&)>&& callback);

		static void find_user_by_id(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);

		static void find_user_by_name(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);

        static void get_user(const HttpRequest& request, std::function<void(const HttpResponse&)>&& callback);
    };
}
