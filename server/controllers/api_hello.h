#pragma once

#include <drogon/HttpController.h>

#include "models/drogon_specialization.hpp"
#include "models/nlohmann_json_request.hpp"
#include "models/nlohmann_json_response.hpp"

using namespace drogon;

namespace api {
    class Hello final : public drogon::HttpController<Hello> {
    public:
        METHOD_LIST_BEGIN
        METHOD_ADD(Hello::say, "/say", Options, Get);
        METHOD_ADD(Hello::echo, "/echo?message={1:message}", Options, Get);
        METHOD_ADD(Hello::hello, "/hello", Options, Post);
        METHOD_ADD(Hello::test_json_body, "/test_json_body", Options, Post);
        METHOD_ADD(Hello::test_request, "/test_request", Options, Post);
        METHOD_ADD(Hello::get_user_model, "/schema/user", Options, Get);
        METHOD_ADD(Hello::get_group_model, "/schema/group", Options, Get);
        METHOD_ADD(Hello::get_permission_model, "/schema/permission", Options, Get);
        METHOD_ADD(Hello::get_operation_model, "/schema/operation", Options, Get);
        METHOD_ADD(Hello::get_date_source_model, "/schema/data_source", Options, Get);
        METHOD_LIST_END

        static void say(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);

        static void echo(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback,
                         const std::string &message);

        static void hello(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);

        static void test_json_body(model_delight::NlohmannJsonRequestPtr &&ptr,
                                   std::function<void(const HttpResponsePtr &)> &&callback);

        static void test_request(model_delight::NlohmannJsonRequestPtr &&ptr,
                                 std::function<void(const HttpResponsePtr &)> &&callback);

        static void get_user_model(model_delight::NlohmannJsonRequestPtr &&ptr,
            std::function<void(const HttpResponsePtr &)> &&callback);

        static void get_group_model(model_delight::NlohmannJsonRequestPtr &&ptr,
            std::function<void(const HttpResponsePtr &)> &&callback);

        static void get_permission_model(model_delight::NlohmannJsonRequestPtr &&ptr,
            std::function<void(const HttpResponsePtr &)> &&callback);

        static void get_operation_model(model_delight::NlohmannJsonRequestPtr &&ptr,
            std::function<void(const HttpResponsePtr &)> &&callback);

        static void get_date_source_model(model_delight::NlohmannJsonRequestPtr &&ptr,
            std::function<void(const HttpResponsePtr &)> &&callback);
    };
} // namespace api
