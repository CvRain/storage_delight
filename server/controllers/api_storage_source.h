#pragma once

#include <drogon/HttpController.h>

#include "basic_value.hpp"
#include "filters/user_filter.hpp"
#include "models/drogon_specialization.hpp"
#include "models/nlohmann_json_request.hpp"

using namespace drogon;

namespace api {
    class StorageSource final : public drogon::HttpController<StorageSource> {
    public:
        METHOD_LIST_BEGIN
        METHOD_ADD(StorageSource::add_source, "/add", Options, Put);
        METHOD_ADD(StorageSource::remove_source, "/remove", Options, Delete);
        METHOD_ADD(StorageSource::list_all_source, "/list", Options, Get);
        METHOD_ADD(StorageSource::list_all_ids, "/ids", Options, Get);
        METHOD_ADD(StorageSource::get_one_source, "/one", Options, Get);

        METHOD_LIST_END
        static void add_source(model_delight::NlohmannJsonRequestPtr        &&request,
                               std::function<void(const HttpResponsePtr &)> &&callback);

        static void remove_source(model_delight::NlohmannJsonRequestPtr        &&request,
                                  std::function<void(const HttpResponsePtr &)> &&callback);

        static void list_all_source(model_delight::NlohmannJsonRequestPtr        &&request,
                                    std::function<void(const HttpResponsePtr &)> &&callback);

        static void list_all_ids(model_delight::NlohmannJsonRequestPtr        &&request,
                                 std::function<void(const HttpResponsePtr &)> &&callback);

        static void get_one_source(model_delight::NlohmannJsonRequestPtr        &&request,
                                   std::function<void(const HttpResponsePtr &)> &&callback);
    };
}  // namespace api
