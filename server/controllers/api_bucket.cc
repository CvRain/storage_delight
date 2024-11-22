#include "api_bucket.h"

#include "drogon_specialization.hpp"
#include "service/bucket_service.hpp"
#include "service/group_service.hpp"
#include "service/log_service.hpp"
#include "service/logger.hpp"
#include "service/storage_service.hpp"

using namespace api;

void Bucket::add_bucket(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
    service_delight::Logger::get_instance().log(service_delight::ConsoleLogger, "Enter Bucket::add bucket");

    const auto json_body = fromRequest<nlohmann::json>(*req);
    if (const auto result = json_body.contains(schema::key::user_id) and
                            json_body.contains(schema::key::bucket_name) and json_body.contains(schema::key::source_id);
        result == false)
    {
        model_delight::BasicResponse response{
                .code = k400BadRequest, .message = "k400BadRequest", .result = "json body is invalid", .data = {}};
        service_delight::Logger::get_instance().log(
                service_delight::ConsoleLogger, "json body is invalid {}", json_body.dump());
        callback(newHttpJsonResponse(response.to_json()));
        return;
    }

    const auto user_id     = json_body.at(schema::key::user_id).get<std::string>();
    const auto bucket_name = json_body.at(schema::key::bucket_name).get<std::string>();
    const auto source_id   = bsoncxx::oid{json_body.at(schema::key::source_id).get<std::string>()};

    try {
        // 检查source是否存在
        if (const auto [is_exist, error] = service_delight::StorageService::get_instance().is_exist(source_id);
            not is_exist.has_value() || is_exist.value() == false)
        {
            service_delight::Logger::get_instance().log(
                    service_delight::ConsoleLogger, spdlog::level::info, "Bucket::add_bucket {}", error);
            throw exception::BaseException{model_delight::BasicResponse{
                    .code = k400BadRequest, .message = "k400BadRequest", .result = error, .data = {}}};
        }

        // 检查数据库中是否存在bucket
        const auto [is_exist, is_exist_error] = service_delight::BucketService::get_instance().is_exist(bucket_name);
        if (not is_exist.has_value()) {
            service_delight::Logger::get_instance().log(
                    service_delight::ConsoleLogger, spdlog::level::info, "Bucket::add_bucket {}", is_exist_error);
            throw exception::BaseException{model_delight::BasicResponse{.code    = k500InternalServerError,
                                                                        .message = "k500InternalServerError",
                                                                        .result  = is_exist_error,
                                                                        .data    = {}}};
        }

        if (is_exist.value() == true) {
            service_delight::Logger::get_instance().log(
                    service_delight::ConsoleLogger, spdlog::level::info, "Bucket::add_bucket {}", "bucket is exist");
            throw exception::BaseException{model_delight::BasicResponse{
                    .code = k400BadRequest, .message = "k400BadRequest", .result = "bucket is exist", .data = {}}};
        }

        // 获得source client
        const auto client = service_delight::StorageService::get_instance().generate_client(source_id);
        if (client == nullptr) {
            service_delight::Logger::get_instance().log(service_delight::ConsoleLogger | service_delight::BasicLogger,
                                                        spdlog::level::err,
                                                        "Bucket::add_bucket {}",
                                                        "client is nullptr");
            throw exception::BaseException{model_delight::BasicResponse{
                    .code    = k500InternalServerError,
                    .message = "k500InternalServerError",
                    .result  = "client nullptr",
            }};
        }
        service_delight::Logger::get_instance().log(
                service_delight::ConsoleLogger, spdlog::level::debug, "Bucket::add_bucket: get client success");

        // minio::s3::MakeBucketArgs args;
        // args.bucket = bucket_name;
        if (const auto make_bucket_response = client->get_bucket_operation()->make_bucket(bucket_name);
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
                service_delight::BucketService::get_instance().add_one(source_id, bucket_name);
        if (not insert_result.has_value()) {
            service_delight::Logger::get_instance().log(
                    service_delight::ConsoleLogger, spdlog::level::err, "Bucket::add_bucket", error);

            auto response = model_delight::BasicResponse{.code    = k500InternalServerError,
                                                         .message = "k500InternalServerError",
                                                         .result  = error.data(),
                                                         .data    = {}};
            throw exception::BaseException{response};
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
                                                                       {schema::key::source_name, source_id.to_string()},
                                                     }};
        callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response.to_json())));

        // 创建操作日志
        schema::DbOperationLog operation_log{};
        operation_log.action        = "add bucket";
        operation_log.bucket_name   = bucket_name;
        operation_log.source_id     = source_id;
        operation_log.current_state = "success";
        operation_log.description   = "success";
        operation_log.request_ip    = req->getPeerAddr().toIp();
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
        operation_log.source_id     = source_id;
        operation_log.current_state = "failed";
        operation_log.description   = e.what();
        operation_log.request_ip    = req->getPeerAddr().toIp();
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
        operation_log.source_id     = source_id;
        operation_log.current_state = "failed";
        operation_log.description   = e.what();
        operation_log.request_ip    = req->getPeerAddr().toIp();
        operation_log.timestamp     = util_delight::Date::get_current_timestamp_32();
        operation_log.user_id       = bsoncxx::oid{user_id};
        service_delight::LogService::get_instance().record_operation(&operation_log);
    }
    service_delight::Logger::get_instance().log(service_delight::ConsoleLogger, "Exit Bucket::add_bucket");
}

void Bucket::list_buckets(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
    service_delight::Logger::get_instance().log(service_delight::ConsoleLogger, "Enter Bucket::list_buckets");

    try {
        const auto request_body = fromRequest<nlohmann::json>(*req);
        const auto user_id      = request_body.at(schema::key::user_id).get<std::string>();
        const auto source_id    = request_body.at(schema::key::source_id).get<std::string>();

        const auto client = service_delight::StorageService::get_instance().get_client(bsoncxx::oid{source_id});
        for (const auto  list_bucket_response = client.value()->get_bucket_operation()->list_buckets();
             const auto& bucket: list_bucket_response.buckets)
        {
            service_delight::Logger::get_instance().log(
                    service_delight::BasicLogger, "Bucket::list_buckets bucket: {}", bucket.name);
        }

        const auto [bucket_document,bucket_document_err] = service_delight::BucketService::get_instance().list();
        //todo
    }
    catch (const std::exception& e) {
        service_delight::Logger::get_instance().log(service_delight::BasicLogger | service_delight::ConsoleLogger,
                                                    spdlog::level::err,
                                                    "Bucket::list_buckets {}",
                                                    e.what());
        model_delight::BasicResponse response{
                .code = k500InternalServerError, .message = "k500InternalServerError", .result = e.what(), .data = {}};
        callback(model_delight::NlohmannResponse::new_nlohmann_json_response(response.to_json()));
    }
}

void Bucket::remove_bucket(model_delight::NlohmannJsonRequestPtr&&       req,
                           std::function<void(const HttpResponsePtr&)>&& callback) {
    service_delight::Logger::get_instance().log(service_delight::ConsoleLogger, "Enter Bucket::remove_bucket");
}

void Bucket::set_owner(model_delight::NlohmannJsonRequestPtr&& req, std::function<void(const HttpResponsePtr&)>&& callback){
}
