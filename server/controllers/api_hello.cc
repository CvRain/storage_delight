#include "api_hello.h"

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
