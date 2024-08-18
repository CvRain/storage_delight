#include "api_hello.h"
#include "models/base_response.hpp"
#include "utils/format.h"
#include <spdlog/spdlog.h>

using namespace api;

// Add definition of your processing function here
void Hello::say(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    Json::Value ret;
    ret["result"] = "Ok";
    ret["message"] = "Hello, world!";
    ret["code"] = 200;
    const auto response = HttpResponse::newHttpJsonResponse(ret);
    callback(response);
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
