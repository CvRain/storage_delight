//
// Created by cvrain-thinkbook on 24-8-18.The base64_decode function uses OpenSSL's BIO functions to decode Base64 strings.

//

#include "user_filter.hpp"

#include "http_response.hpp"
#include "nlohmann_json_response.hpp"
#include "utils/string.hpp"
#include "models/type.hpp"
#include "service/sqlite_service.h"

namespace drogon::middleware {
    void LoginMiddleware::invoke(const HttpRequestPtr &req, MiddlewareNextCallback &&nextCb, MiddlewareCallback &&mcb) {
        spdlog::info("Enter HttpMiddleware<UserFilter>");
        auto token = req->getHeader("Authorization");

        if (token.empty()) {
            spdlog::warn("Token is empty");
            auto response_json = model_delight::HttpResponse{}
                    .set_code(k404NotFound)
                    .set_result("Error")
                    .set_message("Token is empty");
            mcb(model_delight::NlohmannResponse::new_common_response(&response_json));
            return;
        }

        schema::Jwt jwt_token;
        try {
            jwt_token = util_delight::StringEncryption::parse_jwt(token,
                                                                  util_delight::StringEncryption::secret_string).value();
            spdlog::info("Parse jwt token completed");
        } catch (const std::invalid_argument &e) {
            spdlog::warn("JWT parsing failed: {}", e.what());
            auto response_json = model_delight::HttpResponse{}
                    .set_code(k404NotFound)
                    .set_result("Error")
                    .set_message(e.what());
            mcb(model_delight::NlohmannResponse::new_common_response(&response_json));
            return;
        } catch (const std::exception &e) {
            spdlog::error("Unexpected error: {}", e.what());
            auto response_json = model_delight::HttpResponse{}
                    .set_code(k404NotFound)
                    .set_result("Error")
                    .set_message("Token is invalid");
            mcb(model_delight::NlohmannResponse::new_common_response(&response_json));
            return;
        }

        schema::JwtBody jwt_body;
        jwt_body.header.typ = jwt_token.header.at("typ").get<std::string>();
        jwt_body.header.alg = jwt_token.header.at("alg").get<std::string>();

        if (jwt_body.header.typ != "JWT" || jwt_body.header.alg != "HS256") {
            auto response_json = model_delight::HttpResponse{}
                    .set_code(k404NotFound)
                    .set_result("Error")
                    .set_message("Token is invalid");
            mcb(model_delight::NlohmannResponse::new_common_response(&response_json));
            return;
        }

        jwt_body.payload.iss = jwt_token.payload.at("iss").get<std::string>();
        jwt_body.payload.sub = jwt_token.payload.at("sub").get<std::string>();
        jwt_body.payload.aud = jwt_token.payload.at("aud").get<int>();
        jwt_body.payload.exp = jwt_token.payload.at("exp").get<int>();
        jwt_body.payload.iat = jwt_token.payload.at("iat").get<int>();
        jwt_body.payload.user_id = jwt_token.payload.at("user_id").get<int>();

        const auto current_time = util_delight::Date::get_current_timestamp_32();
        if (current_time - jwt_body.payload.iat > jwt_body.payload.exp) {
            auto response_json = model_delight::HttpResponse{}
                    .set_code(k404NotFound)
                    .set_result("Error")
                    .set_message("Token is expired");
            mcb(model_delight::NlohmannResponse::new_common_response(&response_json));
            return;
        }
        spdlog::info("Completion time calibration");

        if (jwt_body.payload.iss != "storage_delight" || jwt_body.payload.sub != "login") {
            auto response_json = model_delight::HttpResponse{}
                    .set_code(k404NotFound)
                    .set_result("Error")
                    .set_message("Token is invalid");
            mcb(model_delight::NlohmannResponse::new_common_response(&response_json));
            return;
        }

        const auto sql_user = SqliteServiceProvider::get_instance()
                .get_sqlite_service()
                .get_user_by_id(jwt_body.payload.user_id);

        if (!sql_user.has_value() || sql_user.value().id != jwt_body.payload.user_id) {
            auto response_json = model_delight::HttpResponse{}
                    .set_code(k404NotFound)
                    .set_result("Error")
                    .set_message("User and token are not matched");
            mcb(model_delight::NlohmannResponse::new_common_response(&response_json));
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