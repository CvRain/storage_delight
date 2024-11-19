#pragma once

#include <drogon/HttpController.h>

#include "nlohmann_json_request.hpp"
#include "nlohmann_json_response.hpp"

using namespace drogon;

namespace api {
    class Bucket final : public drogon::HttpController<Bucket> {
    public:
        METHOD_LIST_BEGIN
        // use METHOD_ADD to add your custom processing function here;
        // METHOD_ADD(bucket::get, "/{2}/{1}", Get); // path is /api/bucket/{arg2}/{arg1}
        // METHOD_ADD(bucket::your_method_name, "/{1}/{2}/list", Get); // path is /api/bucket/{arg1}/{arg2}/list
        // ADD_METHOD_TO(bucket::your_method_name, "/absolute/path/{1}/{2}/list", Get); // path is
        // /absolute/path/{arg1}/{arg2}/list

        METHOD_LIST_END
        static void add_bucket(model_delight::NlohmannJsonRequestPtr        &&req,
                               std::function<void(const HttpResponsePtr &)> &&callback);

        static void remove_bucket(model_delight::NlohmannJsonRequestPtr        &&req,
                                  std::function<void(const HttpResponsePtr &)> &&callback);

        static void set_owner(model_delight::NlohmannJsonRequestPtr        &&req,
                              std::function<void(const HttpResponsePtr &)> &&callback);
    };
}  // namespace api
