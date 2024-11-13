#ifndef DROGON_SPECIALIZATION_HPP
#define DROGON_SPECIALIZATION_HPP

#include <drogon/HttpRequest.h>
#include <spdlog/spdlog.h>

#include "nlohmann_json_request.hpp"
#include "nlohmann_json_response.hpp"

namespace drogon {
    template<>
    inline model_delight::NlohmannJsonRequestPtr fromRequest(const HttpRequest &request) {
        return std::make_shared<model_delight::NlohmannJsonRequest>(request);
    }

    template<>
    inline HttpResponsePtr toResponse<model_delight::TestResponse>(model_delight::TestResponse &&response) {
        Json::Value value;
        value["code"] = response.code;
        value["message"] = response.message;
        return drogon::HttpResponse::newHttpJsonResponse(value);
    }

    template<>
    inline HttpResponsePtr toResponse<nlohmann::json>(nlohmann::json &&value) {
        Json::Value json_value;
        if(Json::Reader json_reader; !json_reader.parse(value.dump(), json_value)){
            spdlog::error("Failed to parse json");
            return drogon::HttpResponse::newHttpJsonResponse(Json::Value::null);
        }
        if(!json_value.isObject()){
            spdlog::error("Invalid json");
            return drogon::HttpResponse::newHttpJsonResponse(Json::Value::null);
        }

        return drogon::HttpResponse::newHttpJsonResponse(json_value);
    }
}


#endif // DROGON_SPECIALIZATION_HPP