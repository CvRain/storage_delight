#ifndef DROGON_SPECIALIZATION_HPP
#define DROGON_SPECIALIZATION_HPP

#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <spdlog/spdlog.h>

#include "nlohmann_json_request.hpp"
#include "nlohmann_json_response.hpp"

namespace drogon {
    template<>
    inline model_delight::NlohmannJsonRequestPtr fromRequest(const HttpRequest &request) {
        return std::make_shared<model_delight::NlohmannJsonRequest>(request);
    }

    inline HttpRequestPtr newHttpJsonRequest(nlohmann::json &&json) {
        auto req = HttpRequest::newHttpRequest();
        req->setContentTypeCode(drogon::CT_APPLICATION_JSON);
        req->setBody(std::move(json.dump()));
        return req;
    }

    template <>
    inline std::shared_ptr<nlohmann::json> fromRequest(const HttpRequest &req) {
        if (const auto& json_ptr = req.jsonObject()) {
            return std::make_shared<nlohmann::json>(req.body());
        }
        return {}; // 返回空的JSON对象
    }

    template <>
    inline nlohmann::json fromRequest<nlohmann::json>(const HttpRequest &req) {
        if (const auto& json_ptr = req.jsonObject()) {
            return nlohmann::json{req.body()};
        }
        return {}; // 返回空的JSON对象
    }

    template <>
    inline HttpRequestPtr toRequest<nlohmann::json>(nlohmann::json &&json) {
        return newHttpJsonRequest(std::move(json));
    }

    template <>
    inline HttpRequestPtr toRequest(const nlohmann::json &j) {
        auto req = HttpRequest::newHttpRequest();
        req->setContentTypeCode(CT_APPLICATION_JSON);
        req->setBody(j.dump()); // 将nlohmann/json对象转换为字符串
        return req;
    }

    inline HttpResponsePtr newHttpJsonResponse(nlohmann::json &&json) {
        auto response = HttpResponse::newHttpResponse();
        response->setContentTypeCode(drogon::CT_APPLICATION_JSON);
        response->setBody(json.dump());
        return response;
    }

    template<>
    inline HttpResponsePtr toResponse<nlohmann::json>(nlohmann::json &&j) {
        auto resp = HttpResponse::newHttpResponse();
        resp->setContentTypeCode(CT_APPLICATION_JSON);
        resp->setBody(j.dump()); // 将nlohmann/json对象转换为字符串
        return resp;
    }

    template<>
    inline HttpResponsePtr toResponse<const nlohmann::json& >(const nlohmann::json &j) {
        auto resp = HttpResponse::newHttpResponse();
        resp->setContentTypeCode(CT_APPLICATION_JSON);
        resp->setBody(j.dump()); // 将nlohmann/json对象转换为字符串
        return resp;
    }

    inline HttpResponsePtr newHttpJsonResponse(const nlohmann::json& json) {
        return toResponse<const nlohmann::json& >(json);
    }
}  // namespace drogon


#endif  // DROGON_SPECIALIZATION_HPP
