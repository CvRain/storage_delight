//
// Created by cvrain on 24-12-2.
//

#include "bucket_exist.hpp"

#include "drogon_specialization.hpp"
#include "schema_key.hpp"
#include "service/group_service.hpp"
#include "service/logger.hpp"
#include "service/storage_service.hpp"
#include "utils/exception_handler.hpp"

void drogon::middleware::BucketExist::invoke(const HttpRequestPtr&    req,
                                             MiddlewareNextCallback&& nextCb,
                                             MiddlewareCallback&&     mcb) {
    try {
        service_delight::Logger::get_instance().log(
            service_delight::ConsoleLogger, spdlog::level::debug, "Enter middleware::BucketExist::invoke");

        const auto json_body   = fromRequest<nlohmann::json>(*req);
        const auto source_id   = json_body.at(schema::key::source_id).get<std::string>();
        const auto bucket_name = json_body.at(schema::key::bucket_name).get<std::string>();

        const auto client = service_delight::StorageService::get_instance().get_client(bsoncxx::oid{source_id}).value();
        const auto exist_response = client->get_bucket_operation()->bucket_exists(bucket_name);
        if (!exist_response) {
            throw exception::BaseException{{.code    = k500InternalServerError,
                                            .message = "k500InternalServerError",
                                            .result  = exist_response.Error().String(),
                                            .data    = {}}};
        }
        if (!exist_response.exist) {
            throw exception::BaseException{{.code    = k400BadRequest,
                                            .message = "k400BadRequest",
                                            .result  = "Bucket does not exist",
                                            .data    = {}}};
        }
        service_delight::Logger::get_instance().log(
            service_delight::ConsoleLogger, spdlog::level::debug, "Leave middleware::BucketExist::invoke");
        nextCb([&, mcb = std::move(mcb)](const HttpResponsePtr& resp) { mcb(resp); });
    }
    catch (const std::exception& e) {
        exception::ExceptionHandler::handle(req, std::move(mcb), e);
    }
}
