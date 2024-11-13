//
// Created by cvrain on 24-9-3.
//

#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <string>
#include <nlohmann/json.hpp>
#include <drogon/HttpResponse.h>

namespace model_delight {
    class HttpResponse {
    public:
        virtual nlohmann::json to_json();

        virtual drogon::HttpResponsePtr to_response();

        virtual ~HttpResponse() = default;

        virtual HttpResponse& set_code(int code);

        virtual HttpResponse& set_message(std::string message);

        virtual HttpResponse& set_result(std::string result);

        int code{};
        std::string message;
        std::string result;
    };
} // model_delight

#endif //HTTP_RESPONSE_HPP
