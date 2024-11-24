//
// Created by cvrain-thinkbook on 24-8-18.The base64_decode function uses OpenSSL's BIO functions to decode Base64
// strings.

//

#include "user_filter.hpp"

#include "models/basic_value.hpp"
#include "models/type.hpp"
#include "nlohmann_json_response.hpp"
#include "schema_key.hpp"
#include "service/logger.hpp"
#include "service/user_service.hpp"
#include "utils/string.hpp"

namespace drogon::middleware {
    void LoginMiddleware::invoke(const HttpRequestPtr    &request,
                                 MiddlewareNextCallback &&nextCb,
                                 MiddlewareCallback     &&mcb) {
        service_delight::Logger::get_instance().log(
                service_delight::ConsoleLogger, spdlog::level::debug, "Enter LoginMiddleware::invoke");

        auto token = request->getHeader("Authorization");

        if (token.empty()) {
            service_delight::Logger::get_instance().log(service_delight::LogOutput::ConsoleLogger,
                                                        spdlog::level::debug,
                                                        "Try to pass login middleware failed: Token is empty");

            nlohmann::json response{{model_delight::basic_value::request::code, k404NotFound},
                                    {model_delight::basic_value::request::message, "Token is empty"},
                                    {model_delight::basic_value::request::result, "Error"}};
            mcb(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));
            return;
        }

        schema::Jwt jwt_token;
        try {
            jwt_token = util_delight::StringEncryption::parse_jwt(token, util_delight::StringEncryption::secret_string)
                                .value();
        }
        catch (const std::invalid_argument &e) {
            service_delight::Logger::get_instance().log(
                    service_delight::LogOutput::ConsoleLogger, "Try to pass login middleware failed: {}", e.what());

            model_delight::BasicResponse response{
                    .code = k404NotFound, .message = "k404NotFound", .result = "Token is invalid", .data = {}};
            mcb(toResponse(response.to_json()));

            return;
        }
        catch (const std::exception &e) {
            service_delight::Logger::get_instance().log(
                    service_delight::LogOutput::ConsoleLogger, "Try to pass login middleware failed: {}", e.what());
            nlohmann::json response{{model_delight::basic_value::request::code, k404NotFound},
                                    {model_delight::basic_value::request::message, e.what()},
                                    {model_delight::basic_value::request::result, "Error"}};
            mcb(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));
            return;
        }

        schema::JwtBody jwt_body;
        jwt_body.header.typ = jwt_token.header.at(model_delight::basic_value::jwt::typ).get<std::string>();
        jwt_body.header.alg = jwt_token.header.at(model_delight::basic_value::jwt::alg).get<std::string>();

        if (jwt_body.header.typ != "JWT" || jwt_body.header.alg != "HS256") {
            nlohmann::json response{{model_delight::basic_value::request::code, k404NotFound},
                                    {model_delight::basic_value::request::message, "Token is invalid"},
                                    {model_delight::basic_value::request::result, "Error"}};
            return;
        }

        jwt_body.payload.iss     = jwt_token.payload.at(model_delight::basic_value::jwt::iss).get<std::string>();
        jwt_body.payload.sub     = jwt_token.payload.at(model_delight::basic_value::jwt::sub).get<std::string>();
        jwt_body.payload.aud     = jwt_token.payload.at(model_delight::basic_value::jwt::aud).get<int>();
        jwt_body.payload.exp     = jwt_token.payload.at(model_delight::basic_value::jwt::exp).get<int>();
        jwt_body.payload.iat     = jwt_token.payload.at(model_delight::basic_value::jwt::iat).get<int>();
        jwt_body.payload.user_id = jwt_token.payload.at(schema::key::user_id).get<std::string>();

        if (const auto current_time = util_delight::Date::get_current_timestamp_32();
            current_time - jwt_body.payload.iat > jwt_body.payload.exp)
        {
            nlohmann::json response{{model_delight::basic_value::request::code, k404NotFound},
                                    {model_delight::basic_value::request::message, "Token is expired"},
                                    {model_delight::basic_value::request::result, "Error"}};
            mcb(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));
            return;
        }

        if (jwt_body.payload.iss != "storage_delight" || jwt_body.payload.sub != "login") {
            nlohmann::json response{{model_delight::basic_value::request::code, k404NotFound},
                                    {model_delight::basic_value::request::message, "Token is invalid"},
                                    {model_delight::basic_value::request::result, "Error"}};
            mcb(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));
            return;
        }

        const auto user_id           = bsoncxx::oid{jwt_body.payload.user_id};
        const auto [user, error_str] = service_delight::UserService::get_instance().get_by_id(user_id);

        if (!user.has_value()) {
            nlohmann::json response{{model_delight::basic_value::request::code, k404NotFound},
                                    {model_delight::basic_value::request::message, error_str},
                                    {model_delight::basic_value::request::result, "Error"}};
            mcb(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));
            return;
        }

        if (const auto fetched_user_id = user.value().view()[schema::key::bson_id].get_oid().value.to_string();
            fetched_user_id != jwt_body.payload.user_id)
        {
            nlohmann::json response{{model_delight::basic_value::request::code, k404NotFound},
                                    {model_delight::basic_value::request::message, "User and token are not matched"},
                                    {model_delight::basic_value::request::result, "Error"}};
            mcb(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));
            return;
        }

        // 只在身份验证成功时调用 nextCb
        nextCb([&, mcb = std::move(mcb)](const HttpResponsePtr &resp) {
            mcb(resp);
            spdlog::info("HttpMiddleware<UserFilter> MiddlewareNextCallback");
        });

        spdlog::info("Exit HttpMiddleware<UserFilter>");
    }
} // filter