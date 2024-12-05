#include "api_object.h"

#include <filesystem>
#include <fstream>
#include <sstream>

#include "drogon_specialization.hpp"
#include "models/nlohmann_json_response.hpp"
#include "service/log_service.hpp"
#include "service/logger.hpp"
#include "service/storage_service.hpp"
#include "utils/exception_handler.hpp"
using namespace api;

void Object::upload(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    service_delight::Logger::get_instance().log(
            service_delight::ConsoleLogger, spdlog::level::debug, "Enter Object::upload");
    try {
        MultiPartParser multi_part_parser;
        multi_part_parser.parse(req);

        const auto &source_id   = multi_part_parser.getParameter<std::string>(schema::key::source_id);
        const auto &bucket_name = multi_part_parser.getParameter<std::string>(schema::key::bucket_name);
        const auto &user_id     = multi_part_parser.getParameter<std::string>(schema::key::user_id);
        const auto &files       = multi_part_parser.getFiles();

        const auto client =
                service_delight::StorageService::get_instance().get_client(bsoncxx::oid{source_id}).value_or(nullptr);
        if (client == nullptr) {
            service_delight::Logger::get_instance().log(service_delight::ConsoleLogger | service_delight::BasicLogger,
                                                        spdlog::level::err,
                                                        "connect to client failed");
            throw exception::BaseException{{.code    = k503ServiceUnavailable,
                                            .message = "k503ServiceUnavailable",
                                            .result  = "connect to client failed",
                                            .data    = {}}};
        }

        nlohmann::json data{};
        nlohmann::json file_names_json{};
        for (const auto &file: files) {
            const auto &file_name    = file.getFileName();
            const auto &file_size    = file.fileLength();
            const auto &file_content = file.fileContent();

            nlohmann::json put_result{
                    {"object_name", file_name},
                    {"object_size", file_size},
            };
            file_names_json["name"].push_back(file_name);

            const auto put_object_response =
                    client->get_object_operation()->put_object(bucket_name, file_name, file_content);
            if (!put_object_response) {
                const auto &error_str = put_object_response.Error().String();
                service_delight::Logger::get_instance().log(
                        service_delight::ConsoleLogger | service_delight::BasicLogger,
                        spdlog::level::warn,
                        "put object failed");
                put_result["status"] = "failed";
                put_result["result"] = error_str;
            }
            else {
                put_result["status"] = "success";
                put_result["result"] = "Ok";
            }
            data.push_back(put_result);
        }
        model_delight::BasicResponse response{
                .code = k201Created, .message = "k201Created", .result = "Ok", .data = data};
        callback(newHttpJsonResponse(response.to_json()));

        // record operation
        schema::DbOperationLog operation_log{};
        operation_log.action        = "upload object";
        operation_log.bucket_name  = bucket_name;
        operation_log.object_name  = file_names_json.dump();
        operation_log.user_id      = bsoncxx::oid{user_id};
        operation_log.source_id    = bsoncxx::oid{source_id};
        service_delight::LogService::get_instance().record_operation(&operation_log);
    }
    catch (const std::exception &exception) {
        exception::ExceptionHandler::handle(req, std::move(callback), exception);
    }
}

void Object::download(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    service_delight::Logger::get_instance().log(
            service_delight::ConsoleLogger, spdlog::level::debug, "Enter Object::download");
    try {
        const auto &request_body = fromRequest<nlohmann::json>(*req);
        const auto &source_id    = request_body.at(schema::key::source_id).get<std::string>();
        const auto &bucket_name  = request_body.at(schema::key::bucket_name).get<std::string>();
        const auto &object_name  = request_body.at(schema::key::object_name).get<std::string>();

        const auto client = service_delight::StorageService::get_instance().get_client(bsoncxx::oid{source_id}).value();
        if (client == nullptr) {
            service_delight::Logger::get_instance().log(service_delight::ConsoleLogger | service_delight::BasicLogger,
                                                        spdlog::level::err,
                                                        "connect to client failed");
            throw exception::BaseException{{.code    = k503ServiceUnavailable,
                                            .message = "k503ServiceUnavailable",
                                            .result  = "connect to client failed",
                                            .data    = {}}};
        }

        auto list_object_result = client->get_object_operation()->list_objects(bucket_name);
        if (!list_object_result) {
            service_delight::Logger::get_instance().log(service_delight::ConsoleLogger | service_delight::BasicLogger,
                                                        spdlog::level::warn,
                                                        "list object failed");
            throw exception::BaseException{{.code    = k503ServiceUnavailable,
                                            .message = "k503ServiceUnavailable",
                                            .result  = "list object failed: nullptr",
                                            .data    = {}}};
        }

        uint file_size{};
        bool is_selected_file = false;
        for (; list_object_result; ++list_object_result) {
            const auto& item = *list_object_result;
            if (not item) {
                continue;
            }
            if (item.name == object_name) {
                file_size = item.size;
                is_selected_file = true;
                break;
            }
        }

        if (is_selected_file == false) {
            service_delight::Logger::get_instance().log(service_delight::ConsoleLogger | service_delight::BasicLogger,
                                                        spdlog::level::warn,
                                                        "list object failed");
            throw exception::BaseException{{.code    = k404NotFound,
                                            .message = "k404NotFound",
                                            .result  = "list object failed: not found",
                                            .data    = {}}};
        }

        const std::filesystem::path temp_save_path =
                std::filesystem::path("./downloads") / source_id / bucket_name / object_name;

        std::filesystem::path base_path   = std::filesystem::path("./downloads") / source_id;
        std::filesystem::path bucket_path = base_path / bucket_name;

        if (not std::filesystem::exists(base_path)) {
            std::filesystem::create_directories(base_path);
        }

        if (not std::filesystem::exists(bucket_path)) {
            std::filesystem::create_directories(bucket_path);
        }
        if(std::filesystem::exists(temp_save_path)) {
            std::filesystem::remove(temp_save_path);
        }

        minio::s3::DownloadObjectArgs download_args;
        download_args.filename     = temp_save_path;
        download_args.bucket       = bucket_name;
        download_args.object       = object_name;
        download_args.progressfunc = [&](const minio::http::ProgressFunctionArgs &args) -> bool {
            spdlog::debug("download total bytes: {}", args.download_total_bytes);
            return true;
        };
        if (const auto download_object_response = client->get_object_operation()->download_object(download_args);
            not download_object_response)
        {
            const auto &error_str = download_object_response.Error().String();
            service_delight::Logger::get_instance().log(service_delight::ConsoleLogger | service_delight::BasicLogger,
                                                        spdlog::level::warn,
                                                        "download object failed {}",
                                                        error_str);
            throw exception::BaseException{{.code    = k503ServiceUnavailable,
                                            .message = "k503ServiceUnavailable",
                                            .result  = error_str,
                                            .data    = {}}};
        }

        callback(HttpResponse::newFileResponse(
                temp_save_path, 0, file_size, true, object_name, CT_NONE));
    }
    catch (const std::exception &exception) {
        exception::ExceptionHandler::handle(req, std::move(callback), exception);
    }
}

void Object::list_objects(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    service_delight::Logger::get_instance().log(
            service_delight::ConsoleLogger, spdlog::level::debug, "Enter Object::list_objects");
    try {
        const auto &request_body = fromRequest<nlohmann::json>(*req);
        const auto &source_id    = request_body.at(schema::key::source_id).get<std::string>();
        const auto &bucket_name  = request_body.at(schema::key::bucket_name).get<std::string>();

        const auto client =
                service_delight::StorageService::get_instance().get_client(bsoncxx::oid{source_id}).value_or(nullptr);
        if (client == nullptr) {
            service_delight::Logger::get_instance().log(service_delight::ConsoleLogger | service_delight::BasicLogger,
                                                        spdlog::level::err,
                                                        "connect to client failed");
            throw exception::BaseException{{.code    = k503ServiceUnavailable,
                                            .message = "k503ServiceUnavailable",
                                            .result  = "connect to client failed",
                                            .data    = {}}};
        }

        auto list_object_result = client->get_object_operation()->list_objects(bucket_name);
        if (!list_object_result) {
            service_delight::Logger::get_instance().log(service_delight::ConsoleLogger | service_delight::BasicLogger,
                                                        spdlog::level::warn,
                                                        "list object failed");
            throw exception::BaseException{{.code    = k503ServiceUnavailable,
                                            .message = "k503ServiceUnavailable",
                                            .result  = "list object failed: nullptr",
                                            .data    = {}}};
        }

        nlohmann::json data{};
        for (; list_object_result; ++list_object_result) {
            auto item = *list_object_result;
            if (not item) {
                continue;
            }
            nlohmann::json object_info{{"name", item.name},
                                       {"version_id", item.version_id},
                                       {"etag", item.etag},
                                       {"size", item.size},
                                       {"last_modified", item.last_modified.ToISO8601UTC()}

            };
            data.push_back(object_info);
        }

        model_delight::BasicResponse response{.code = k200OK, .message = "k200OK", .result = "OK", .data = data};
        callback(newHttpJsonResponse(response.to_json()));
    }
    catch (const std::exception &exception) {
        exception::ExceptionHandler::handle(req, std::move(callback), exception);
    }
}

void Object::remove_objects(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    service_delight::Logger::get_instance().log(
            service_delight::ConsoleLogger, spdlog::level::debug, "Enter Object::remove_objects");
    try {
        const auto &request_body = fromRequest<nlohmann::json>(*req);
        const auto &source_id    = request_body.at(schema::key::source_id).get<std::string>();
        const auto &bucket_name  = request_body.at(schema::key::bucket_name).get<std::string>();
        const auto &object_name  = request_body.at(schema::key::object_name).get<std::string>();

        const auto client =
                service_delight::StorageService::get_instance().get_client(bsoncxx::oid{source_id}).value_or(nullptr);
        if (client == nullptr) {
            service_delight::Logger::get_instance().log(service_delight::ConsoleLogger | service_delight::BasicLogger,
                                                        spdlog::level::err,
                                                        "connect to client failed");
            throw exception::BaseException{{.code    = k503ServiceUnavailable,
                                            .message = "k503ServiceUnavailable",
                                            .result  = "connect to client failed",
                                            .data    = {}}};
        }
        if (const auto remove_object_result = client->get_object_operation()->remove_object(bucket_name, object_name);
            !remove_object_result)
        {
            service_delight::Logger::get_instance().log(service_delight::ConsoleLogger | service_delight::BasicLogger,
                                                        spdlog::level::warn,
                                                        "remove object failed {}",
                                                        remove_object_result.Error().String());
            throw exception::BaseException{{.code    = k503ServiceUnavailable,
                                            .message = "k503ServiceUnavailable",
                                            .result  = remove_object_result.Error().String(),
                                            .data    = {}}};
        }
        model_delight::BasicResponse response{.code = k200OK, .message = "k200OK", .result = "OK", .data = {}};
        callback(newHttpJsonResponse(response.to_json()));

        // todo record operation
    }
    catch (const std::exception &exception) {
        exception::ExceptionHandler::handle(req, std::move(callback), exception);
    }
}

void Object::rename(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    service_delight::Logger::get_instance().log(
            service_delight::ConsoleLogger, spdlog::level::debug, "Enter Object::rename");
    try {
        const auto &request_body = fromRequest<nlohmann::json>(*req);
        const auto &source_id    = request_body.at(schema::key::source_id).get<std::string>();
        const auto &bucket_name  = request_body.at(schema::key::bucket_name).get<std::string>();
        const auto &object_name  = request_body.at(schema::key::object_name).get<std::string>();
        const auto &new_name     = request_body.at("new_name").get<std::string>();

        const auto client = service_delight::StorageService::get_instance().get_client(bsoncxx::oid{source_id}).value();
        if (client == nullptr) {
            service_delight::Logger::get_instance().log(service_delight::ConsoleLogger | service_delight::BasicLogger,
                                                        spdlog::level::err,
                                                        "connect to client failed");
            throw exception::BaseException{{.code    = k503ServiceUnavailable,
                                            .message = "k503ServiceUnavailable",
                                            .result  = "connect to client failed",
                                            .data    = {}}};
        }

        const auto copy_object_response =
                client->get_object_operation()->copy_object(bucket_name, object_name, bucket_name, new_name);
        if (not copy_object_response) {
            service_delight::Logger::get_instance().log(service_delight::ConsoleLogger | service_delight::BasicLogger,
                                                        spdlog::level::warn,
                                                        "rename object failed {}",
                                                        copy_object_response.Error().String());

            throw exception::BaseException{{.code    = k503ServiceUnavailable,
                                            .message = "k503ServiceUnavailable rename failed failed",
                                            .result  = copy_object_response.Error().String(),
                                            .data    = {}}};
        }

        if (const auto remove_object_response = client->get_object_operation()->remove_object(bucket_name, object_name);
            not remove_object_response)
        {
            service_delight::Logger::get_instance().log(service_delight::ConsoleLogger | service_delight::BasicLogger,
                                                        spdlog::level::warn,
                                                        "remove object failed {}",
                                                        remove_object_response.Error().String());

            throw exception::BaseException{{.code    = k503ServiceUnavailable,
                                            .message = "k503ServiceUnavailable remove failed failed",
                                            .result  = remove_object_response.Error().String(),
                                            .data    = {}}};
        }
    }
    catch (const std::exception &exception) {
        exception::ExceptionHandler::handle(req, std::move(callback), exception);
    }
}
