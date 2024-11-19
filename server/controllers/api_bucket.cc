#include "api_bucket.h"

#include "service/bucket_service.hpp"
#include "service/group_service.hpp"
#include "service/log_service.hpp"
#include "service/logger.hpp"
#include "service/storage_service.hpp"
#include "service/user_service.hpp"

using namespace api;

void Bucket::add_bucket(model_delight::NlohmannJsonRequestPtr&&       req,
                        std::function<void(const HttpResponsePtr&)>&& callback) {
    service_delight::Logger::get_instance().log(service_delight::ConsoleLogger, "Enter Bucket::add bucket");

    // 检查json是否包括必要字段
    const auto json_body = req->getNlohmannJsonBody();
    if (const auto result = json_body.contains(schema::key::user_id) and
                            json_body.contains(schema::key::bucket_name) and
                            json_body.contains(schema::key::source_name);
        result == false)
    {
        model_delight::BasicResponse response{
                .code = k400BadRequest, .message = "k400BadRequest", .result = "json body is invalid", .data = {}};
        callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response.to_json())));
    }

    const auto user_id     = json_body.at(schema::key::user_id).get<std::string>();
    const auto bucket_name = json_body.at(schema::key::bucket_name).get<std::string>();
    const auto source_name = json_body.at(schema::key::source_name).get<std::string>();

    try {
        // 检查source_name 是否存在
        if (const auto [is_exist, error] = service_delight::StorageService::get_instance().is_exist(source_name);
            not is_exist.has_value() || is_exist.value() == false)
        {
            service_delight::Logger::get_instance().log(
                    service_delight::ConsoleLogger, spdlog::level::info, "Bucket::add_bucket {}", error);
            throw exception::BaseException{model_delight::BasicResponse{
                    .code = k400BadRequest, .message = "k400BadRequest", .result = error, .data = {}}};
        }

        // 检查数据库中是否存在bucket
        if (const auto [is_exist, error] = service_delight::BucketService::get_instance().is_exist(bucket_name);
            not is_exist.has_value() || is_exist.value() == false)
        {
            service_delight::Logger::get_instance().log(
                    service_delight::ConsoleLogger, spdlog::level::info, "Bucket::add_bucket {}", error);
            throw exception::BaseException{model_delight::BasicResponse{
                    .code = k400BadRequest, .message = "k400BadRequest", .result = error, .data = {}}};
        }

        // 获得source client
        const auto client = service_delight::StorageService::get_instance().get_client(source_name);

        // 在存储源中创建桶
        if (const auto make_bucket_response = client.value()->get_bucket_operation().make_bucket(bucket_name);
            !make_bucket_response)
        {
            service_delight::Logger::get_instance().log(service_delight::ConsoleLogger | service_delight::BasicLogger,
                                                        spdlog::level::err,
                                                        "Bucket::add_bucket {}",
                                                        make_bucket_response.Error().String());
            throw exception::BaseException{model_delight::BasicResponse{.code    = k500InternalServerError,
                                                                        .message = "k500InternalServerError",
                                                                        .result = make_bucket_response.Error().String(),
                                                                        .data   = {}}};
        }

        // 在数据库中记录bucket
        const auto [insert_result, error] =
                service_delight::BucketService::get_instance().add_one(source_name, bucket_name);
        if (not insert_result.has_value()) {
            service_delight::Logger::get_instance().log(service_delight::ConsoleLogger, spdlog::level::err, "Bucket::add_bucket", error);

            auto response = model_delight::BasicResponse{.code    = k500InternalServerError,
                                                         .message = "k500InternalServerError",
                                                         .result  = error.data(),
                                                         .data    = {}};
            throw exception::BaseException{std::move(response)};
        }

        service_delight::Logger::get_instance().log(service_delight::ConsoleLogger, "Bucket::add bucket success");

        // 返回响应
        const auto insert_id = insert_result.value().to_string();
        auto       response  = model_delight::BasicResponse{.code    = k200OK,
                                                            .message = "k200OK",
                                                            .result  = "success",
                                                            .data    = nlohmann::json{
                                                                       {schema::key::bucket_id, insert_id},
                                                                       {schema::key::bucket_name, bucket_name},
                                                                       {schema::key::source_name, source_name},
                                                     }};
        callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response.to_json())));

        // 创建操作日志
        schema::DbOperationLog operation_log{};
        operation_log.action        = "add bucket";
        operation_log.bucket_name   = bucket_name;
        operation_log.source_name   = source_name;
        operation_log.current_state = "success";
        operation_log.description   = "success";
        operation_log.request_ip    = req->getRequest().peerAddr().toIp();
        operation_log.timestamp     = util_delight::Date::get_current_timestamp_32();
        operation_log.user_id       = bsoncxx::oid{user_id};
        service_delight::LogService::get_instance().record_operation(&operation_log);
        service_delight::Logger::get_instance().log(service_delight::ConsoleLogger, "Bucket::add_bucket success");
    }
    catch (const exception::BaseException& e) {
        service_delight::Logger::get_instance().log(service_delight::BasicLogger | service_delight::ConsoleLogger,
                                                    spdlog::level::err,
                                                    "Bucket::add_bucket {}",
                                                    e.what());
        callback(model_delight::NlohmannResponse::new_nlohmann_json_response(e.response().to_json()));

        schema::DbOperationLog operation_log{};
        operation_log.action        = "add bucket";
        operation_log.bucket_name   = bucket_name;
        operation_log.source_name   = source_name;
        operation_log.current_state = "failed";
        operation_log.description   = e.what();
        operation_log.request_ip    = req->getRequest().peerAddr().toIp();
        operation_log.timestamp     = util_delight::Date::get_current_timestamp_32();
        operation_log.user_id       = bsoncxx::oid{user_id};

        service_delight::LogService::get_instance().record_operation(&operation_log);
    }
    catch (const std::exception& e) {
        service_delight::Logger::get_instance().log(service_delight::BasicLogger | service_delight::ConsoleLogger,
                                                    spdlog::level::err,
                                                    "Bucket::add_bucket {}",
                                                    e.what());
        model_delight::BasicResponse response{
                .code = k500InternalServerError, .message = "k500InternalServerError", .result = e.what(), .data = {}};
        callback(model_delight::NlohmannResponse::new_nlohmann_json_response(response.to_json()));

        schema::DbOperationLog operation_log{};
        operation_log.action        = "add bucket";
        operation_log.bucket_name   = bucket_name;
        operation_log.source_name   = source_name;
        operation_log.current_state = "failed";
        operation_log.description   = e.what();
        operation_log.request_ip    = req->getRequest().peerAddr().toIp();
        operation_log.timestamp     = util_delight::Date::get_current_timestamp_32();
        operation_log.user_id       = bsoncxx::oid{user_id};
        service_delight::LogService::get_instance().record_operation(&operation_log);
    }
    service_delight::Logger::get_instance().log(service_delight::ConsoleLogger, "Exit Bucket::add_bucket");
}
