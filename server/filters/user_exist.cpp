//
// Created by cvrain on 24-11-17.
//

#include "user_exist.hpp"

#include "schema_key.hpp"
#include "service/logger.hpp"
#include "service/user_service.hpp"
#include "type.hpp"
#include "drogon_specialization.hpp"

namespace drogon::middleware {
    void UserExist::invoke(const HttpRequestPtr& request, MiddlewareNextCallback&& nextCb, MiddlewareCallback&& mcb) {
        service_delight::Logger::get_instance().log(service_delight::ConsoleLogger, "Enter UserExist::invoke");
        try {
            // 检查是否存在schema::key::user_id元素
            if (request->getBody().empty() || not request->getJsonObject()->isMember(schema::key::user_id)) {
                throw exception::BaseException{model_delight::BasicResponse{
                        .code = k400BadRequest, .message = "Invalid request", .result = "Request key not found"}};
            }

            const auto body    = *request->getJsonObject();
            const auto user_id = bsoncxx::oid{body[schema::key::user_id].asString()};

            const auto [user_exist_result, error] = service_delight::UserService::get_instance().is_exist(user_id);
            if (not user_exist_result.has_value()) {
                throw exception::BaseException{model_delight::BasicResponse{
                        .code = k500InternalServerError, .message = "k500InternalServerError", .result = error}};
            }
            if (user_exist_result.value() == false) {
                throw exception::BaseException{model_delight::BasicResponse{
                        .code = k400BadRequest, .message = "k400BadRequest", .result = "User not exist", .data{}}};
            }
            service_delight::Logger::get_instance().log(service_delight::ConsoleLogger,
                                                        "Enter UserExist::invoke: check completed!");
            nextCb([&, mcb = std::move(mcb)](const HttpResponsePtr& resp) { mcb(resp); });
        }
        catch (const exception::BaseException& e) {
            service_delight::Logger::get_instance().log(
                    service_delight::ConsoleLogger, spdlog::level::info, "UserExist::invoke: {}", e.what());

            mcb(toResponse<const nlohmann::json&>((e.response().to_json())));
        }
        catch (const std::exception& e) {
            service_delight::Logger::get_instance().log(
                    service_delight::ConsoleLogger, spdlog::level::err, "UserExist::invoke: {}", e.what());

            const auto response =
                    model_delight::BasicResponse{
                            .code    = k500InternalServerError,
                            .message = e.what(),
                            .result  = "k500InternalServerError",
                            .data{},
                    }
                            .to_json();
            mcb(toResponse<const nlohmann::json&>(response));
        }
    }
}  // namespace drogon::middleware
