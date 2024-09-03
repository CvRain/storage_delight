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

    static drogon::HttpResponsePtr newNlohmannJsonResponse(nlohmann::json &&json)
    {
        return drogon::HttpResponse::newCustomHttpResponse<nlohmann::json>(std::move(json));
    }

}
#endif //STORAGE_DELIGHT_NLOHMANN_JSON_RESPONSE_HPP
