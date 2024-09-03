//
// Created by cvrain on 24-9-3.
//

#ifndef STORAGE_DELIGHT_NLOHMANN_JSON_RESPONSE_HPP
#define STORAGE_DELIGHT_NLOHMANN_JSON_RESPONSE_HPP

#include <drogon/HttpResponse.h>
#include <nlohmann/json.hpp>
#include <json/json.h>
#include <spdlog/spdlog.h>

namespace model_delight{
    static drogon::HttpResponsePtr newHttpNlohmannJsonResponse(nlohmann::json &&value){
        std::stringstream json_value_stream;
        json_value_stream << value.dump();

        Json::Value result;
        Json::CharReaderBuilder builder;
        std::string errors;

        const auto parsing_success = Json::parseFromStream(builder, json_value_stream, &result, &errors);
        if(!parsing_success){
            spdlog::error("Failed to parse json string {}", errors);
            throw std::runtime_error("Failed to parse json string " + errors);
        }
        return drogon::HttpResponse::newHttpJsonResponse(result);
    }
}

#endif //STORAGE_DELIGHT_NLOHMANN_JSON_RESPONSE_HPP
