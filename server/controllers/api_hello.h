#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

namespace api {
    class Hello : public drogon::HttpController<Hello> {
    public:
        METHOD_LIST_BEGIN
            METHOD_ADD(Hello::say, "/say", Get);
            METHOD_ADD(Hello::echo, "/echo?message={1:message}", Get);
            METHOD_ADD(Hello::hello, "/hello", Post);
        METHOD_LIST_END

        static void say(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
        static void echo(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, const std::string& message);
        static void hello(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);

    };
}
