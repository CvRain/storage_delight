#include "api_hello.h"

#include <spdlog/spdlog.h>

#include "db_schema.hpp"
#include "models/drogon_specialization.hpp"
#include "schema_key.hpp"
#include "utils/date.h"

using namespace api;

auto Hello::say(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) -> void {
    callback(model_delight::NlohmannResponse::new_common_response(
            &model_delight::HttpResponse{}
                     .set_code(k200OK)
                     .set_message(std::to_string(util_delight::Date::get_current_timestamp_32()))
                     .set_result("ok")));
}

void Hello::echo(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback,
                 const std::string &message) {
    callback(model_delight::NlohmannResponse::new_common_response(
            &model_delight::HttpResponse{}.set_code(k200OK).set_message(message).set_result("ok")));
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

void Hello::test_json_body(model_delight::NlohmannJsonRequestPtr &&ptr,
                           std::function<void(const HttpResponsePtr &)> &&callback) {
    spdlog::info("Enter Hello::test_json_body");
    spdlog::info("{}", ptr->getNlohmannJsonBody().dump());
    callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(ptr->getNlohmannJsonBody())));

    spdlog::info("Exit Hello::test_json_body");
}

void Hello::test_request(model_delight::NlohmannJsonRequestPtr &&ptr,
                         std::function<void(const HttpResponsePtr &)> &&callback) {
    spdlog::info("Enter Hello::test_request");

    // callback(model_delight::NlohmannResponse::new_nlohmann_json_response(ptr->getNlohmannJsonBody()));
    const auto response = std::make_shared<model_delight::HttpResponse>();
    response->set_result("K200OK").set_code(HttpStatusCode::k200OK).set_message("OK");
    callback(model_delight::NlohmannResponse::new_common_response(response.get()));
    spdlog::info("Exit Hello::test_request");
}

void Hello::get_user_model(model_delight::NlohmannJsonRequestPtr &&ptr,
                           std::function<void(const HttpResponsePtr &)> &&callback) {
    schema::DbUser user;
    user.group_id = bsoncxx::oid{};
    user.id = bsoncxx::oid{};
    user.name = "test";
    user.password = "test";
    user.role = static_cast<int32_t>(schema::UserRole::TypeUser);
    user.update_time = 0;
    user.create_time = util_delight::Date::get_current_timestamp_32();
    callback(model_delight::NlohmannResponse::new_nlohmann_json_response(user.to_json()));

}

void Hello::get_bucket_model(model_delight::NlohmannJsonRequestPtr &&ptr,
                             std::function<void(const HttpResponsePtr &)> &&callback) {
    schema::DbBucket bucket;
    bucket.bucket_name = "test bucket";
    bucket.data_source = bsoncxx::oid{};
    bucket.group_id = bsoncxx::oid{};
    bucket.id = bsoncxx::oid{};
    bucket.permission_id = bsoncxx::oid{};
    bucket.tags = {"tag1", "tag2", "tag3"};
    callback(model_delight::NlohmannResponse::new_nlohmann_json_response(bucket.to_json()));
}

void Hello::get_group_model(model_delight::NlohmannJsonRequestPtr &&ptr,
                            std::function<void(const HttpResponsePtr &)> &&callback) {
    schema::DbGroup group;
    group.bucket_group_id = {bsoncxx::oid{}, bsoncxx::oid{}};
    group.members_id = {bsoncxx::oid{}, bsoncxx::oid{}};
    group.name = "test group";
    group.owner_id = bsoncxx::oid{};
    callback(model_delight::NlohmannResponse::new_nlohmann_json_response(group.to_json()));
}

void Hello::get_permission_model(model_delight::NlohmannJsonRequestPtr &&ptr,
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
    permission.name = "test permission";
    permission.description = "test permission description";
    callback(model_delight::NlohmannResponse::new_nlohmann_json_response(permission.to_json()));
}

void Hello::get_operation_model(model_delight::NlohmannJsonRequestPtr &&ptr,
                                std::function<void(const HttpResponsePtr &)> &&callback) {
    schema::DbOperationLog operation;
    operation.action = "test action";
    operation.bucket_name = "test bucket";
    operation.current_state = "test current state";
    operation.description = "test description";
    operation.object_name = "test object name";
    operation.previous_state = "test previous state";
    operation.request_ip = ptr->getRequest().getPeerAddr().toIp();
    operation.timestamp = util_delight::Date::get_current_timestamp_32();
    callback(model_delight::NlohmannResponse::new_nlohmann_json_response(operation.to_json()));
}

void Hello::get_date_source_model(model_delight::NlohmannJsonRequestPtr &&ptr,
                                  std::function<void(const HttpResponsePtr &)> &&callback) {
    schema::DbDataSource data_source;
    data_source.access_key = "test access key";
    data_source.create_time = 0;
    data_source.id = bsoncxx::oid{};
    data_source.name = "test name";
    data_source.secret_key = "test secret key";
    data_source.url = "test url";
    data_source.is_https = false;
    callback(model_delight::NlohmannResponse::new_nlohmann_json_response(data_source.to_json()));
}
