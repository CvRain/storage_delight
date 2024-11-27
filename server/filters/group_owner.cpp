//
// Created by cvrain on 24-11-27.
//

#include "group_owner.hpp"

#include "drogon_specialization.hpp"
#include "schema_key.hpp"
#include "service/group_service.hpp"
#include "service/logger.hpp"

void drogon::middleware::GroupOwner::invoke(const HttpRequestPtr&    req,
                                            MiddlewareNextCallback&& nextCb,
                                            MiddlewareCallback&&     mcb) {
    const auto& json_body = fromRequest<nlohmann::json>(*req);
    service_delight::Logger::get_instance().log(
            service_delight::ConsoleLogger, spdlog::level::debug, "Enter middleware::GroupOwner::invoke");
    try {
        const auto& group_id = json_body.at(schema::key::group_id).get<std::string>();
        const auto& user_id  = json_body.at(schema::key::user_id).get<std::string>();

        if (const auto group = service_delight::GroupService::get_instance().get_one(bsoncxx::oid{group_id});
            group.first.value().owner_id != bsoncxx::oid{user_id}) {
            service_delight::Logger::get_instance().log(service_delight::ConsoleLogger,
                                                        spdlog::level::err,
                                                        "User {} is not the owner of group {}",
                                                        user_id,
                                                        group_id);
            model_delight::BasicResponse response{.code    = k403Forbidden,
                                                  .message = "k403Forbidden",
                                                  .result  = "User is not the owner of group",
                                                  .data    = {}};
            mcb(newHttpJsonResponse(std::move(response.to_json())));
            return;
        }
        service_delight::Logger::get_instance().log(service_delight::ConsoleLogger,
                                                    spdlog::level::debug,
                                                    "User {} is the owner of group {}",
                                                    user_id,
                                                    group_id);
        nextCb([&, mcb = std::move(mcb)](const HttpResponsePtr& resp) { mcb(resp); });
    }
    catch (const nlohmann::detail::exception& exception) {
        service_delight::Logger::get_instance().log(service_delight::ConsoleLogger,
                                                    spdlog::level::err,
                                                    "Error in middleware::GroupExist::invoke {}",
                                                    exception.what());
        model_delight::BasicResponse response{
                .code = k400BadRequest, .message = "k400BadRequest", .result = exception.what(), .data = {}};
        mcb(newHttpJsonResponse(std::move(response.to_json())));
    }
    catch (const exception::BaseException& exception) {
        service_delight::Logger::get_instance().log(service_delight::ConsoleLogger,
                                                    spdlog::level::err,
                                                    "Error in middleware::GroupExist::invoke {}",
                                                    exception.what());
        mcb(newHttpJsonResponse(std::move(exception.response().to_json())));
    }
    catch (const std::exception& exception) {
        service_delight::Logger::get_instance().log(service_delight::ConsoleLogger,
                                                    spdlog::level::err,
                                                    "Error in middleware::GroupExist::invoke {}",
                                                    exception.what());
        model_delight::BasicResponse response{.code    = k500InternalServerError,
                                              .message = "k500InternalServerError",
                                              .result  = exception.what(),
                                              .data    = {}};
        mcb(newHttpJsonResponse(std::move(response.to_json())));
    }
}
