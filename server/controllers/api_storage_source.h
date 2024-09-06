#pragma once

#include <drogon/HttpController.h>

#include "models/drogon_specialization.hpp"
#include "models/nlohmann_json_request.hpp"
#include "models/nlohmann_json_response.hpp"
#include "filters/user_filter.hpp"

using namespace drogon;

namespace api {
    class StorageSource : public drogon::HttpController<StorageSource> {
    public:
        METHOD_LIST_BEGIN
        METHOD_ADD(StorageSource::add_source, "/add", HttpMethod::Put, middleware::LoginMiddlewareName);

        METHOD_LIST_END
        void add_source(model_delight::NlohmannJsonRequestPtr &&request,
                        std::function<void(const HttpResponsePtr &)> &&callback);
    };
}
