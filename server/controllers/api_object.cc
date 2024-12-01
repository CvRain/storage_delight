#include "api_object.h"

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

        // todo
        nlohmann::json data{};
        for (const auto &file: files) {
            data.push_back({{"object_name", file.getFileName()},
                            {"object_size", file.fileLength()},
                            {"object_md5", file.getMd5()}});
        }
        model_delight::BasicResponse response{
                .code = k201Created, .message = "k201Created", .result = "Ok", .data = data};
        callback(newHttpJsonResponse(response.to_json()));
    }
    catch (const std::exception &exception) {
        exception::ExceptionHandler::handle(req, std::move(callback), exception);
    }
}

void Object::download(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    service_delight::Logger::get_instance().log(
            service_delight::ConsoleLogger, spdlog::level::debug, "Enter Object::download");

    try {
    }
    catch (const std::exception &exception) {
        exception::ExceptionHandler::handle(req, std::move(callback), exception);
    }
}
