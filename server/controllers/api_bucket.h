#pragma once

#include <drogon/HttpController.h>

#include "nlohmann_json_request.hpp"
#include "nlohmann_json_response.hpp"

using namespace drogon;
using model_delight::basic_value::middleware::UserExist;

namespace api {
    class Bucket final : public HttpController<Bucket> {
    public:
        METHOD_LIST_BEGIN
        METHOD_ADD(Bucket::add_bucket, "/add", Options, Put);
        METHOD_ADD(Bucket::list_bucket, "/list", Options, Get);
        METHOD_ADD(Bucket::remove_bucket, "/remove", Options, Delete, Post);
        METHOD_ADD(Bucket::all_bucket, "/all", Options, Get, model_delight::basic_value::middleware::UserExist);
        METHOD_LIST_END

        static void add_bucket(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
        static void list_bucket(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
        static void remove_bucket(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
        static void all_bucket(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
    };
}  // namespace api
