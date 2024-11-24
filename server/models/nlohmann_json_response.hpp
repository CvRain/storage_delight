//
// Created by cvrain on 24-9-3.
//

#ifndef STORAGE_DELIGHT_NLOHMANN_JSON_RESPONSE_HPP
#define STORAGE_DELIGHT_NLOHMANN_JSON_RESPONSE_HPP

#include <drogon/HttpResponse.h>
#include <nlohmann/json.hpp>

#include "db_schema.hpp"
#include "basic_value.hpp"

namespace model_delight {
    struct TestResponse {
        int code;
        std::string message;
    };

    struct BasicResponse {
        int code;
        std::string_view message;
        std::string_view result;
        nlohmann::json data{};

        nlohmann::json to_json() {
            nlohmann::json json{
                    {model_delight::basic_value::request::code, code},
                    {model_delight::basic_value::request::message, message},
                    {model_delight::basic_value::request::result, result},
                    {"data", data}
            };
            return json;
        }
    };


    class NlohmannResponse {
    public:
        static drogon::HttpResponsePtr new_nlohmann_json_response(nlohmann::json&&json) {
            return drogon::HttpResponse::newCustomHttpResponse<nlohmann::json>(std::move(json));
        }
    };
}
#endif //STORAGE_DELIGHT_NLOHMANN_JSON_RESPONSE_HPP
