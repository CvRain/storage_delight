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
        METHOD_LIST_END

        static void add_bucket(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);

        static void remove_bucket(model_delight::NlohmannJsonRequestPtr        &&req,
                                  std::function<void(const HttpResponsePtr &)> &&callback);

        static void set_owner(model_delight::NlohmannJsonRequestPtr        &&req,
                              std::function<void(const HttpResponsePtr &)> &&callback);
    };
}  // namespace api
