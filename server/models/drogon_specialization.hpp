#ifndef DROGON_SPECIALIZATION_HPP
#define DROGON_SPECIALIZATION_HPP

#include <drogon/HttpRequest.h>
#include <spdlog/spdlog.h>

#include "nlohmann_json_request.hpp"
#include "nlohmann_json_response.hpp"

namespace drogon{
    template<>
    inline model_delight::NlohmannJsonRequestPtr fromRequest(const HttpRequest& request){
        return std::make_shared<model_delight::NlohmannJsonRequest>(request);
    }

    template<>
    inline HttpResponsePtr toResponse<model_delight::TestResponse>(model_delight::TestResponse &&response){
        Json::Value value;
        value["code"] = response.code;
        value["message"] = response.message;
        return drogon::HttpResponse::newHttpJsonResponse(value);
    }

    template<>
    inline HttpResponsePtr toResponse<nlohmann::json>(nlohmann::json &&value){
        std::stringstream json_value_stream;
        json_value_stream << value.dump();

        Json::Value result;
        Json::CharReaderBuilder builder;
        std::string errors;

        if (Json::parseFromStream(builder, json_value_stream, &result, &errors)) {
            spdlog::error("Failed to parse json string {}", errors);
            throw std::runtime_error("Failed to parse json string " + errors);
        }
        return drogon::HttpResponse::newHttpJsonResponse(result);
    }
}


#endif // DROGON_SPECIALIZATION_HPP