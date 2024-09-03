#pragma once

#include <drogon/HttpController.h>

#include "models/nlohmann_json_request.hpp"
#include "models/nlohmann_json_response.hpp"
#include "models/drogon_specialization.hpp"

using namespace drogon;

namespace api {
    class Hello : public drogon::HttpController<Hello> {
    public:
        METHOD_LIST_BEGIN
            METHOD_ADD(Hello::say, "/say", Get);
            METHOD_ADD(Hello::echo, "/echo?message={1:message}", Get);
            METHOD_ADD(Hello::hello, "/hello", Post);
            METHOD_ADD(Hello::test_json_body, "/test_json_body", Post);
        METHOD_LIST_END

        static void say(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);

        static void echo(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback,
                         const std::string &message);

        static void hello(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);

        static void test_json_body(model_delight::NlohmannJsonRequestPtr &&ptr,
                                   std::function<void(const HttpResponsePtr &)> &&callback);

    };
}
