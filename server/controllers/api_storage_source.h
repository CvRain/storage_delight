#pragma once

#include <drogon/HttpController.h>

#include "basic_value.hpp"
#include "filters/user_filter.hpp"
#include "models/drogon_specialization.hpp"
#include "models/nlohmann_json_request.hpp"
#include "models/nlohmann_json_response.hpp"

using namespace drogon;

namespace api {
    class StorageSource : public drogon::HttpController<StorageSource> {
    public:
        METHOD_LIST_BEGIN
        METHOD_ADD(StorageSource::add_source, "/add", HttpMethod::Put, model_delight::basic_value::middleware::LoginAuth);

        METHOD_LIST_END
        void add_source(model_delight::NlohmannJsonRequestPtr &&request,
                        std::function<void(const HttpResponsePtr &)> &&callback);
    };
}
