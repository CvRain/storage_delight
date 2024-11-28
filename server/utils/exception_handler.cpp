//
// Created by cvrain on 24-11-28.
//

#include "exception_handler.hpp"

#include "drogon_specialization.hpp"
#include "nlohmann_json_request.hpp"
#include "service/logger.hpp"
#include "type.hpp"

namespace exception {
    void ExceptionHandler::handle(const drogon::HttpRequestPtr&                 request,
                                  std::function<void(const HttpResponsePtr&)>&& callback,
                                  const std::exception&                         exception) {
        if (const auto* nlohmann_exception = dynamic_cast<const nlohmann::detail::exception*>(&exception)) {
            ExceptionHandler::nlohmann_exception(request, std::move(callback), exception);
            return;
        }
        if (const auto* base_exception = dynamic_cast<const BaseException*>(&exception)) {
            ExceptionHandler::base_exception(request, std::move(callback), exception);
            return;
        }
        ExceptionHandler::standard_exception(request, std::move(callback), exception);
    }

    void ExceptionHandler::nlohmann_exception(const drogon::HttpRequestPtr&                 request,
                                              std::function<void(const HttpResponsePtr&)>&& callback,
                                              const std::exception&                         exception) {
        service_delight::Logger::get_instance().log(
                service_delight::ConsoleLogger, spdlog::level::debug, "Enter ExceptionHandler::nlohmann_exception");

        model_delight::BasicResponse response{
                .code = drogon::k400BadRequest, .message = "k400BadRequest", .result = exception.what(), .data = {}};
        callback(newHttpJsonResponse(response.to_json()));
    }

    void ExceptionHandler::base_exception(const drogon::HttpRequestPtr&                 request,
                                          std::function<void(const HttpResponsePtr&)>&& callback,
                                          const std::exception&                         exception) {
        service_delight::Logger::get_instance().log(
                service_delight::ConsoleLogger, spdlog::level::debug, "Enter ExceptionHandler::base_exception");

        const auto* base_exception = dynamic_cast<const BaseException*>(&exception);
        service_delight::Logger::get_instance().log(service_delight::ConsoleLogger | service_delight::BasicLogger,
                                                    spdlog::level::err,
                                                    "ExceptionHandler: {}",
                                                    base_exception->response().result);
        callback(newHttpJsonResponse(base_exception->response().to_json()));
    }

    void ExceptionHandler::standard_exception(const HttpRequestPtr&                         request,
                                              std::function<void(const HttpResponsePtr&)>&& callback,
                                              const std::exception&                         exception) {
        service_delight::Logger::get_instance().log(
                service_delight::ConsoleLogger, spdlog::level::debug, "Enter ExceptionHandler::standard_exception");

        service_delight::Logger::get_instance().log(service_delight::ConsoleLogger | service_delight::BasicLogger,
                                                    spdlog::level::err,
                                                    "ExceptionHandler: {}",
                                                    exception.what());
        callback(newHttpJsonResponse(model_delight::BasicResponse{
                .code    = drogon::k500InternalServerError,
                .message = "k500InternalServerError",
                .result  = exception.what(),
                .data    = {}}.to_json()));
    }

}  // namespace exception
