//
// Created by cvrain on 24-9-3.
//

#ifndef STORAGE_DELIGHT_NLOHMANN_JSON_RESPONSE_HPP
#define STORAGE_DELIGHT_NLOHMANN_JSON_RESPONSE_HPP

#include <drogon/HttpResponse.h>
#include <nlohmann/json.hpp>

#include "http_response.hpp"

namespace model_delight {
    struct TestResponse {
        int code;
        std::string message;
    };


    class NlohmannResponse {
    public:
        static drogon::HttpResponsePtr new_nlohmann_json_response(nlohmann::json&&json) {
            return drogon::HttpResponse::newCustomHttpResponse<nlohmann::json>(std::move(json));
        }

        static drogon::HttpResponsePtr new_common_response(HttpResponse *response) {
            return response->to_response();
        }
    };
}
#endif //STORAGE_DELIGHT_NLOHMANN_JSON_RESPONSE_HPP
