#include "api_hello.h"
#include "models/base_response.hpp"
#include <spdlog/spdlog.h>
#include "models/user_response.hpp"
#include <nlohmann/json.hpp>

using namespace api;

auto Hello::say(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) -> void{
    const auto json = model_delight::CommonResponse()
            .append("message","hello world!")
            .append("result","ok")
            .append("code",k200OK)
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

void Hello::test_json_body(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback,
                           const model_delight::NlohmannJsonRequestPtr& ptr) {
    callback(HttpResponse::newHttpJsonResponse(*(req->getJsonObject())));
}
