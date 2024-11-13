//
// Created by cvrain on 24-9-3.
//

#include "http_response.hpp"
#include "nlohmann_json_response.hpp"

namespace model_delight {
    nlohmann::json HttpResponse::to_json() {
        return nlohmann::json{
            {"code", code},
            {"message", message},
            {"result", result}
        };
    }

    drogon::HttpResponsePtr HttpResponse::to_response() {
        return NlohmannResponse::new_nlohmann_json_response(this->to_json());
    }

    HttpResponse& HttpResponse::set_code(int code) {
        this->code = code;
        return *this;
    }

    HttpResponse& HttpResponse::set_message(std::string message) {
        this->message = std::move(message);
        return *this;
    }

    HttpResponse& HttpResponse::set_result(std::string result) {
        this->result = std::move(result);
        return *this;
    }
} // model_delight
