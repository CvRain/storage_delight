#include "api_bucket.h"

#include "drogon_specialization.hpp"
#include "service/log_service.hpp"
#include "service/logger.hpp"
#include "service/storage_service.hpp"
#include "utils/exception_handler.hpp"

using namespace api;

/**
 * 为用户组添加存储桶
 */
void Bucket::add_bucket(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
    service_delight::Logger::get_instance().log(
            service_delight::ConsoleLogger, spdlog::level::info, "Enter Bucket::add_bucket");
    try {
        const auto request_body = fromRequest<nlohmann::json>(*req);
        auto&& [user_id, source_id, bucket_name] =
                std::make_tuple(request_body.at(schema::key::user_id).get<std::string>(),
                                request_body.at(schema::key::source_id).get<std::string>(),
                                request_body.at(schema::key::bucket_name).get<std::string>());

        // 获取minio client
        const auto client = service_delight::StorageService::get_instance().get_client(bsoncxx::oid{source_id});
        if (not client.has_value() || client.value() == nullptr) {
            service_delight::Logger::get_instance().log(service_delight::ConsoleLogger,
                                                        spdlog::level::err,
                                                        "Error in Bucket::add_bucket: client not found");
            throw exception::BaseException{
                    model_delight::BasicResponse{.code    = k400BadRequest,
                                                 .message = "k400BadRequest",
                                                 .result  = "Error in Bucket::add_bucket: client not found",
                                                 .data    = request_body}};
        }
        // 创建桶
        const auto add_bucket_response = client.value()->get_bucket_operation()->make_bucket(bucket_name);
        if (!add_bucket_response) {
            service_delight::Logger::get_instance().log(service_delight::ConsoleLogger,
                                                        spdlog::level::err,
                                                        "Error in Bucket::add_bucket: {}",
                                                        add_bucket_response.Error().String());
            auto response    = model_delight::BasicResponse{};
            response.code    = k400BadRequest;
            response.message = "k400BadRequest";
            response.result  = add_bucket_response.Error().String();
            callback(newHttpJsonResponse(response.to_json()));
        }
        nlohmann::json info{{schema::key::bucket_name, bucket_name}};

        auto response    = model_delight::BasicResponse{};
        response.code    = k200OK;
        response.message = "k200OK";
        response.result  = "Success";
        response.data    = std::move(info);
        callback(newHttpJsonResponse(response.to_json()));

        service_delight::Logger::get_instance().log(
                service_delight::ConsoleLogger, spdlog::level::info, "Success in Bucket::add_bucket");
        schema::DbOperationLog operation_log;
        operation_log.action        = "add_bucket";
        operation_log.bucket_name   = bucket_name;
        operation_log.current_state = [&]() {
            if (not add_bucket_response) return "Failed";
            return "Success";
        }();
        operation_log.description = add_bucket_response.Error().String();
        operation_log.request_ip  = req->getPeerAddr().toIp();
        operation_log.timestamp   = util_delight::Date::get_current_timestamp_32();
        service_delight::LogService::get_instance().record_operation(&operation_log);
    }
    catch (const std::exception& exception) {
        exception::ExceptionHandler::handle(req, std::move(callback), exception);
    }
}

/**
 * 列出某个数据源下的所有桶
 */
void Bucket::list_bucket(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
    service_delight::Logger::get_instance().log(
            service_delight::ConsoleLogger, spdlog::level::debug, "Start in Bucket::list_bucket");
    try {
        const auto request_body     = fromRequest<nlohmann::json>(*req);
        auto&& [user_id, source_id] = std::make_tuple(request_body.at(schema::key::user_id).get<std::string>(),
                                                      request_body.at(schema::key::source_id).get<std::string>());

        const auto client = service_delight::StorageService::get_instance().get_client(bsoncxx::oid{source_id});
        if (not client.has_value() || client.value() == nullptr) {
            service_delight::Logger::get_instance().log(service_delight::ConsoleLogger,
                                                        spdlog::level::err,
                                                        "Error in Bucket::list_bucket: client not found");
            throw exception::BaseException{
                    model_delight::BasicResponse{.code    = k400BadRequest,
                                                 .message = "k400BadRequest",
                                                 .result  = "Error in Bucket::list_bucket: client not found",
                                                 .data    = request_body}};
        }
        const auto list_bucket_response = client.value()->get_bucket_operation()->list_buckets();
        if (!list_bucket_response) {
            service_delight::Logger::get_instance().log(service_delight::ConsoleLogger,
                                                        spdlog::level::err,
                                                        "Error in Bucket::list_bucket: {}",
                                                        list_bucket_response.Error().String());
            auto response    = model_delight::BasicResponse{};
            response.code    = k400BadRequest;
            response.message = "k400BadRequest";
            response.result  = list_bucket_response.Error().String();
            callback(newHttpJsonResponse(response.to_json()));
            return;
        }
        const auto     bucket_list = list_bucket_response.buckets;
        nlohmann::json bucket_names;
        std::ranges::for_each(bucket_list,
                              [&bucket_names](const auto& bucket) { bucket_names.push_back(bucket.name); });
        auto response = model_delight::BasicResponse{
                .code = k200OK, .message = "k200OK", .result = "Success", .data = bucket_names};
        callback(newHttpJsonResponse(response.to_json()));
        service_delight::Logger::get_instance().log(
                service_delight::ConsoleLogger, spdlog::level::info, "Success in Bucket::list_bucket");
    }
    catch (const std::exception& exception) {
        exception::ExceptionHandler::handle(req, std::move(callback), exception);
    }
}

/**
 * 删除某个数据源下的某个桶
 */
void Bucket::remove_bucket(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
    service_delight::Logger::get_instance().log(
            service_delight::ConsoleLogger, spdlog::level::debug, "Start in Bucket::remove_bucket");
    try {
        const auto request_body     = fromRequest<nlohmann::json>(*req);
        auto&& [user_id, source_id] = std::make_tuple(request_body.at(schema::key::user_id).get<std::string>(),
                                                      request_body.at(schema::key::source_id).get<std::string>());

        const auto client = service_delight::StorageService::get_instance().get_client(bsoncxx::oid{source_id});
        if (not client.has_value() || client.value() == nullptr) {
            service_delight::Logger::get_instance().log(service_delight::ConsoleLogger,
                                                        spdlog::level::err,
                                                        "Error in Bucket::remove_bucket: client not found");
            throw exception::BaseException{
                    model_delight::BasicResponse{.code    = k400BadRequest,
                                                 .message = "k400BadRequest",
                                                 .result  = "Error in Bucket::remove_bucket: client not found",
                                                 .data    = request_body}};
        }
        const auto remove_bucket_response = client.value()->get_bucket_operation()->remove_bucket(
                request_body.at(schema::key::bucket_name).get<std::string>());
        if (!remove_bucket_response) {
            service_delight::Logger::get_instance().log(service_delight::ConsoleLogger,
                                                        spdlog::level::err,
                                                        "Error in Bucket::remove_bucket: {}",
                                                        remove_bucket_response.Error().String());
            auto response    = model_delight::BasicResponse{};
            response.code    = k400BadRequest;
            response.message = "k400BadRequest";
            response.result  = remove_bucket_response.Error().String();
            callback(newHttpJsonResponse(response.to_json()));
            return;
        }
        service_delight::Logger::get_instance().log(
                service_delight::ConsoleLogger, spdlog::level::info, "Success in Bucket::remove_bucket");

        model_delight::BasicResponse response{
                .code = k200OK, .message = "k200OK", .result = "Success", .data = nlohmann::json{}};
        callback(newHttpJsonResponse(std::move(response.to_json())));

        schema::DbOperationLog operation_log{};
        operation_log.action        = "remove_bucket";
        operation_log.bucket_name   = request_body.at(schema::key::bucket_name).get<std::string>();
        operation_log.current_state = "Success";
        operation_log.description   = "Remove bucket";
        operation_log.source_id     = bsoncxx::oid{source_id};
        operation_log.user_id       = bsoncxx::oid{user_id};

        service_delight::LogService::get_instance().record_operation(&operation_log);
    }
    catch (const std::exception& exception) {
        exception::ExceptionHandler::handle(req, std::move(callback), exception);
    }
}

void Bucket::all_bucket(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
    service_delight::Logger::get_instance().log(
            service_delight::ConsoleLogger, spdlog::level::debug, "Start in Bucket::all_bucket");
    try {
        const auto& [sources, error] = service_delight::StorageService::get_instance().list_all_storage_ids();
        if (not sources.has_value()) {
            service_delight::Logger::get_instance().log(
                    service_delight::ConsoleLogger, spdlog::level::err, "Bucket::all_bucket: {}", error);
            throw exception::BaseException{model_delight::BasicResponse{.code    = k500InternalServerError,
                                                                        .message = "k500InternalServerError",
                                                                        .result  = error,
                                                                        .data    = {}}};
        }
        nlohmann::json data;
        for (const auto& source: sources.value()) {
            const auto client = service_delight::StorageService::get_instance().get_client(source);
            if (not client.has_value() || client.value() == nullptr) {
                service_delight::Logger::get_instance().log(
                        service_delight::ConsoleLogger, spdlog::level::err, "Bucket::all_bucket: client not found");
                throw exception::BaseException{
                        model_delight::BasicResponse{.code    = k400BadRequest,
                                                     .message = "k400BadRequest",
                                                     .result  = "Error in Bucket::all_bucket: client not found",
                                                     .data    = {}}};
            }
            const auto bucket_list_response = client.value()->get_bucket_operation()->list_buckets();
            if (not bucket_list_response) {
                service_delight::Logger::get_instance().log(service_delight::ConsoleLogger,
                                                            spdlog::level::err,
                                                            "Bucket::all_bucket: {}",
                                                            bucket_list_response.Error().String());
                throw exception::BaseException{
                        model_delight::BasicResponse{.code    = k400BadRequest,
                                                     .message = "k400BadRequest",
                                                     .result  = bucket_list_response.Error().String()}};
            }
            const auto& buckets    = bucket_list_response.buckets;
            const auto  source_str = source.to_string();
            for (const auto& it: buckets) {
                data.push_back(
                        nlohmann::json{{schema::key::source_id, source_str}, {schema::key::bucket_name, it.name}});
            }
        }
        model_delight::BasicResponse response{.code = k200OK, .message = "k200OK", .result = "Success", .data = data};
        callback(newHttpJsonResponse(std::move(response.to_json())));
    }
    catch (const std::exception& exception) {
        exception::ExceptionHandler::handle(req, std::move(callback), exception);
    }
}
