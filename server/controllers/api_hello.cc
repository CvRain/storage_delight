#include "api_hello.h"
#include "models/base_response.hpp"
#include <spdlog/spdlog.h>
#include "models/user_response.hpp"
#include "models/nlohmann_json_request.hpp"
#include "models/nlohmann_json_response.hpp"
#include "models/drogon_specialization.hpp"

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
    callback(HttpResponse::newCustomHttpResponse(std::move(
            nlohmann::json{
                    {"message", message},
                    {"result",  "ok"},
                    {"code",    k200OK}
            }
    )));
}

void Hello::hello(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    spdlog::info("hello");

    MultiPartParser parser;
    parser.parse(req);
    const auto result = parser.getFiles().at(0);

    const auto text = parser.getParameter<std::string>("text");
    spdlog::info("text: {}", text);

    callback(model_delight::newNlohmannJsonResponse(nlohmann::json{
            {"message", "hello world!"},
            {"result",  "ok"},
            {"code",    k200OK}
    }));
}

void Hello::test_json_body(model_delight::NlohmannJsonRequestPtr &&ptr,
                           std::function<void(const HttpResponsePtr &)> &&callback) {
    spdlog::info("Enter Hello::test_json_body");

    model_delight::TestResponse test_response{
        .code = HttpStatusCode::k200OK,
        .message = "Hello world!"
    };
    callback(HttpResponse::newCustomHttpResponse<model_delight::TestResponse>(std::move(test_response)));

    spdlog::info("Exit Hello::test_json_body");
}
