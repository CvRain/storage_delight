#include "api_object.h"
#include "models/base_response.hpp"
#include "models/nlohmann_json_response.hpp"

using namespace api;

void Object::upload(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    MultiPartParser file_upload;
    if (file_upload.parse(req) != 0 || file_upload.getFiles().size() != 1) {
        callback(model_delight::newNlohmannJsonResponse(
                nlohmann::json{
                        {"code",    400},
                        {"message", "Bad Request"},
                        {"result",  "Invalid file"}
                }
        ));
        return;
    }

    const auto &file = file_upload.getFiles()[0];
    const auto md5 = file.getMd5();

    callback(HttpResponse::newCustomHttpResponse<nlohmann::json>(std::move(nlohmann::json{
            {"code",    200},
            {"message", "OK"},
            {"result",  md5}
    })));
}
