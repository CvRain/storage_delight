#include "api_object.h"
#include "utils/response.h"

using namespace api;

void Object::upload(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)
{
    MultiPartParser file_upload;
    if(file_upload.parse(req) != 0 || file_upload.getFiles().size() != 1){
        auto response = HttpResponse::newHttpJsonResponse(
            util_delight::BaseResponse()
            .set_code(513)
            .set_message("file upload failed")
            .set_result("error")
            .to_jsoncpp_json()
        );
        callback(response);
        return;
    }

    const auto &file = file_upload.getFiles()[0];
    const auto md5 = file.getMd5();

    callback(HttpResponse::newHttpJsonResponse(
        util_delight::BaseResponse()
        .set_code(200)
        .set_message(md5)
        .set_result("Ok")
        .to_jsoncpp_json()
    ));
}