//
// Created by cvrain on 24-11-26.
//

#include "group_exist.hpp"

#include "drogon_specialization.hpp"
#include "schema_key.hpp"
#include "service/group_service.hpp"
#include "service/logger.hpp"
void drogon::middleware::GroupExist::invoke(const HttpRequestPtr&    req,
                                            MiddlewareNextCallback&& nextCb,
                                            MiddlewareCallback&&     mcb) {
    const auto& json_body = fromRequest<nlohmann::json>(*req);
    service_delight::Logger::get_instance().log(
            service_delight::ConsoleLogger, spdlog::level::debug, "Enter middleware::GroupExist::invoke");
    try {
        const auto& group_id = json_body.at(schema::key::group_id).get<std::string>();
        const auto& [find_document, find_error] =
                service_delight::GroupService::get_instance().is_exist(bsoncxx::oid{group_id});
        if (not find_document) {
            throw exception::BaseException{model_delight::BasicResponse{.code    = k500InternalServerError,
                                                                        .message = "k500InternalServerError",
                                                                        .result  = find_error,
                                                                        .data    = {}}};
        }
        if (find_document.value() == false) {
            service_delight::Logger::get_instance().log(
                    service_delight::ConsoleLogger, spdlog::level::debug, "Group {} not found", group_id);
            const model_delight::BasicResponse response{
                    .code = k404NotFound, .message = "k404NotFound", .result = "Group not found", .data = {}};
            throw exception::BaseException{response};
        }

        service_delight::Logger::get_instance().log(
                service_delight::ConsoleLogger, spdlog::level::debug, "Group {} found", group_id);
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
