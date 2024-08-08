#include "api_hello.h"
#include "utils/response.h"
#include <spdlog/spdlog.h>

using namespace api;

// Add definition of your processing function here
void Hello::say(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    Json::Value ret;
    ret["result"] = "Ok";
    ret["message"] = "Hello, world!";
    ret["code"] = 200;
    auto response = HttpResponse::newHttpJsonResponse(ret);
    callback(response);
}

void Hello::echo(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback,
                 const std::string &message) {
    callback(HttpResponse::newHttpJsonResponse(util_delight::BaseResponse{}
                                                       .set_code(k200OK)
                                                       .set_message(message)
                                                       .set_result("Ok")
                                                       .to_jsoncpp_json()));
}

void Hello::hello(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    spdlog::info("hello");

    MultiPartParser parser;
    parser.parse(req);
    const auto result = parser.getFiles().at(0);
    const auto content = result.fileContent();


    const auto text = parser.getParameter<std::string>("text");
    spdlog::info("text: {}", text);

    callback(HttpResponse::newHttpJsonResponse(util_delight::BaseResponse{}
                                                       .set_code(k200OK)
                                                       .set_message(result.getFileName())
                                                       .set_result("Ok")
                                                       .to_jsoncpp_json()));
}
