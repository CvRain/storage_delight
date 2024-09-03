#include "api_hello.h"

#include <spdlog/spdlog.h>
#include "models/user_response.hpp"
#include "models/drogon_specialization.hpp"

using namespace api;

auto Hello::say(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) -> void {
    callback(model_delight::NlohmannResponse::new_common_response(
        &model_delight::HttpResponse{}
        .set_code(k200OK)
        .set_message("Hello world!")
        .set_result("ok")
    ));
}

void Hello::echo(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback,
                 const std::string &message) {
    callback(model_delight::NlohmannResponse::new_common_response(
        &model_delight::HttpResponse{}
        .set_code(k200OK)
        .set_message(message)
        .set_result("ok")
    ));
}

void Hello::hello(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    spdlog::info("hello");

    MultiPartParser parser;
    parser.parse(req);
    const auto result = parser.getFiles().at(0);

    const auto text = parser.getParameter<std::string>("text");
    spdlog::info("text: {}", text);

    callback(model_delight::NlohmannResponse::new_nlohmann_json_response(
        nlohmann::json{
            {"code", 200},
            {"message", "Hello world!"},
            {"result", "ok"},
            {"file", result.getFileName()}
        }
    ));
}

void Hello::test_json_body(model_delight::NlohmannJsonRequestPtr &&ptr,
                           std::function<void(const HttpResponsePtr &)> &&callback) {
    spdlog::info("Enter Hello::test_json_body");

    callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(ptr->getNlohmannJsonBody())));

    spdlog::info("Exit Hello::test_json_body");
}
