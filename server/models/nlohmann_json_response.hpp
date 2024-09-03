//
// Created by cvrain on 24-9-3.
//

#ifndef STORAGE_DELIGHT_NLOHMANN_JSON_RESPONSE_HPP
#define STORAGE_DELIGHT_NLOHMANN_JSON_RESPONSE_HPP

#include <drogon/HttpResponse.h>
#include <nlohmann/json.hpp>
#include <json/json.h>
#include <spdlog/spdlog.h>

namespace model_delight {
    struct TestResponse{
        int code;
        std::string message;
    };

    class HttpResponse {
    public:
        explicit HttpResponse(nlohmann::json &&value)
                : m_value(value) {

        }

        static drogon::HttpResponsePtr newHttpNlohmannJsonResponse(nlohmann::json &&value) {
            std::stringstream json_value_stream;
            json_value_stream << value.dump();

            Json::Value result;
            Json::CharReaderBuilder builder;
            std::string errors;

            const auto parsing_success = Json::parseFromStream(builder, json_value_stream, &result, &errors);
            if (!parsing_success) {
                spdlog::error("Failed to parse json string {}", errors);
                throw std::runtime_error("Failed to parse json string " + errors);
            }
            return drogon::HttpResponse::newHttpJsonResponse(result);
        }
    private:
        nlohmann::json m_value;
    };
}

namespace drogon{
    template<>
    inline HttpResponsePtr toResponse(model_delight::TestResponse &&response){
        Json::Value value;
        value["code"] = response.code;
        value["message"] = response.message;
        return drogon::HttpResponse::newHttpJsonResponse(value);
    }

}

#endif //STORAGE_DELIGHT_NLOHMANN_JSON_RESPONSE_HPP
