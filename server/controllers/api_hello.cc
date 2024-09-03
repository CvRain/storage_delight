#include "api_hello.h"
#include "models/base_response.hpp"
#include <spdlog/spdlog.h>
#include "models/user_response.hpp"
#include "models/nlohmann_json_response.hpp"

using namespace api;

auto Hello::say(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) -> void {
    const auto json = model_delight::CommonResponse()
            .append("message", "hello world!")
            .append("result", "ok")
            .append("code", k200OK)
            .to_json();
    callback(HttpResponse::newHttpJsonResponse(json));
}

void Hello::echo(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback,
                 const std::string &message) {
    const auto json = model_delight::BaseResponse{}
            .set_code(k200OK)
            .set_message(message)
            .set_result("Ok")
            .to_json();
    callback(HttpResponse::newHttpJsonResponse(json));
}

void Hello::hello(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    spdlog::info("hello");

    MultiPartParser parser;
    parser.parse(req);
    const auto result = parser.getFiles().at(0);

    const auto text = parser.getParameter<std::string>("text");
    spdlog::info("text: {}", text);

    const auto json = model_delight::BaseResponse{}
            .set_code(k200OK)
            .set_message(result.getFileName())
            .set_result("Ok")
            .to_json();

    callback(HttpResponse::newHttpJsonResponse(json));
}

void Hello::test_json_body(model_delight::NlohmannJsonRequestPtr &&ptr,
                           std::function<void(const HttpResponsePtr &)> &&callback) {
    spdlog::info("Enter Hello::test_json_body");

    const auto json_body = ptr->getNlohmannJsonBody();
//    callback(HttpResponse::newHttpJsonResponse(
//            model_delight::CommonResponse{}
//            .append("message", "Hello " + json_body["name"].get<std::string>())
//            .append("result", "ok")
//            .append("code", k200OK)
//            .to_json()
//            ));
    callback(model_delight::newHttpNlohmannJsonResponse(nlohmann::json{
            {"message", "Hello " + json_body["name"].get<std::string>()},
            {"result", "ok"},
            {"code", k200OK}
    }));
    spdlog::info("Exit Hello::test_json_body");
}
