#include "api_hello.h"

#include <spdlog/spdlog.h>

#include "db_schema.hpp"
#include "models/drogon_specialization.hpp"
#include "schema_key.hpp"
#include "utils/date.h"

using namespace api;

auto Hello::say(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) -> void {
    auto                 json_body = fromRequest<nlohmann::json>(*req);
    const nlohmann::json response{{"code", k200OK}, {"message", "Hello world!"}, {"result", "k200OK"}};
    callback(newHttpJsonResponse(response));
}

void Hello::echo(const HttpRequestPtr                          &req,
                 std::function<void(const HttpResponsePtr &)> &&callback,
                 const std::string                             &message) {
    const auto request_body = fromRequest<nlohmann::json>(*req);
    auto       response     = model_delight::BasicResponse{
                      .code = k202Accepted, .message = "k202Accepted", .result = "Ok", .data = request_body};
    callback(newHttpJsonResponse(response.to_json()));
}

void Hello::hello(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    spdlog::info("hello");

    MultiPartParser parser;
    parser.parse(req);
    const auto result = parser.getFiles().at(0);

    const auto text = parser.getParameter<std::string>("text");
    spdlog::info("text: {}", text);

    callback(model_delight::NlohmannResponse::new_nlohmann_json_response(nlohmann::json{
            {"code", 200}, {"message", "Hello world!"}, {"result", "ok"}, {"file", result.getFileName()}}));
}

void Hello::test_json_body(model_delight::NlohmannJsonRequestPtr        &&ptr,
                           std::function<void(const HttpResponsePtr &)> &&callback) {
    spdlog::info("Enter Hello::test_json_body");
    spdlog::info("{}", ptr->getNlohmannJsonBody().dump());
    callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(ptr->getNlohmannJsonBody())));

    spdlog::info("Exit Hello::test_json_body");
}

void Hello::test_request(model_delight::NlohmannJsonRequestPtr        &&ptr,
                         std::function<void(const HttpResponsePtr &)> &&callback) {
    spdlog::info("Enter Hello::test_request");
    callback(model_delight::NlohmannResponse::new_nlohmann_json_response(ptr->getNlohmannJsonBody()));
    spdlog::info("Exit Hello::test_request");
}

void Hello::get_user_model(model_delight::NlohmannJsonRequestPtr        &&ptr,
                           std::function<void(const HttpResponsePtr &)> &&callback) {
    schema::DbUser user;
    user.group_id    = bsoncxx::oid{};
    user.id          = bsoncxx::oid{};
    user.name        = "test";
    user.password    = "test";
    user.role        = static_cast<int32_t>(schema::UserRole::TypeUser);
    user.update_time = 0;
    user.create_time = util_delight::Date::get_current_timestamp_32();
    callback(model_delight::NlohmannResponse::new_nlohmann_json_response(user.to_json()));
}

void Hello::get_group_model(model_delight::NlohmannJsonRequestPtr        &&ptr,
                            std::function<void(const HttpResponsePtr &)> &&callback) {
    schema::DbGroup group;
    group.buckets    = {{bsoncxx::oid{}, "test bucket name"}, {bsoncxx::oid{}, "test bucket name2"}};
    group.members_id = {bsoncxx::oid{}, bsoncxx::oid{}};
    group.name       = "test group";
    group.owner_id   = bsoncxx::oid{};
    callback(model_delight::NlohmannResponse::new_nlohmann_json_response(group.to_json()));
}

void Hello::get_permission_model(model_delight::NlohmannJsonRequestPtr        &&ptr,
                                 std::function<void(const HttpResponsePtr &)> &&callback) {
    schema::DbPermission permission;
    permission.allow_actions = {
            {schema::key::permission::allow_read, {bsoncxx::oid{}, bsoncxx::oid{}}},
            {schema::key::permission::allow_create_bucket, {bsoncxx::oid{}, bsoncxx::oid{}}},
            {schema::key::permission::allow_delete_bucket, {bsoncxx::oid{}, bsoncxx::oid{}}},
            {schema::key::permission::allow_update_bucket, {bsoncxx::oid{}, bsoncxx::oid{}}},
            {schema::key::permission::allow_delete, {bsoncxx::oid{}, bsoncxx::oid{}}},
            {schema::key::permission::allow_grant, {bsoncxx::oid{}, bsoncxx::oid{}}},
            {schema::key::permission::allow_list, {bsoncxx::oid{}, bsoncxx::oid{}}},
            {schema::key::permission::allow_write, {bsoncxx::oid{}, bsoncxx::oid{}}},
            {schema::key::permission::allow_revoke, {bsoncxx::oid{}, bsoncxx::oid{}}},
    };
    permission.bucket = std::make_pair(bsoncxx::oid{}, "test bucket name");
    callback(model_delight::NlohmannResponse::new_nlohmann_json_response(permission.to_json()));
}

void Hello::get_operation_model(model_delight::NlohmannJsonRequestPtr        &&ptr,
                                std::function<void(const HttpResponsePtr &)> &&callback) {
    schema::DbOperationLog operation;
    operation.action         = "test action";
    operation.bucket_name    = "test bucket";
    operation.current_state  = "test current state";
    operation.description    = "test description";
    operation.object_name    = "test object name";
    operation.previous_state = "test previous state";
    operation.request_ip     = ptr->getRequest().getPeerAddr().toIp();
    operation.timestamp      = util_delight::Date::get_current_timestamp_32();
    callback(model_delight::NlohmannResponse::new_nlohmann_json_response(operation.to_json()));
}

void Hello::get_date_source_model(model_delight::NlohmannJsonRequestPtr        &&ptr,
                                  std::function<void(const HttpResponsePtr &)> &&callback) {
    schema::DbDataSource data_source;
    data_source.access_key  = "test access key";
    data_source.create_time = 0;
    data_source.id          = bsoncxx::oid{};
    data_source.name        = "test name";
    data_source.secret_key  = "test secret key";
    data_source.url         = "test url";
    data_source.is_https    = false;
    callback(model_delight::NlohmannResponse::new_nlohmann_json_response(data_source.to_json()));
}

void Hello::object_upload(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    MultiPartParser multi_part_parser;
    multi_part_parser.parse(req);
    auto &files = multi_part_parser.getFiles();
    if (files.empty()) {
        model_delight::BasicResponse response{
                .code = k400BadRequest, .message = "k400BadRequest", .result = "No file found"};
        callback(newHttpJsonResponse(response.to_json()));
    }

    nlohmann::json file_info;
    for (const auto &file: files) {
        file_info.push_back(
                nlohmann::json{{"name", file.getFileName()}, {"size", file.fileLength()}, {"md5", file.getMd5()}});
    }
    model_delight::BasicResponse response{
            .code = k202Accepted, .message = "k202Accepted", .result = "OK", .data = std::move(file_info)};
    callback(newHttpJsonResponse(response.to_json()));
}

void Hello::object_download(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {}
