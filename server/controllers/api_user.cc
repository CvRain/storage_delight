#include "api_user.h"
#include "service/sqlite_service.h"
#include "utils/response.h"
#include "utils/string_encryption.h"
#include "nlohmann/json.hpp"

using namespace api;

/// \brief 添加新用户 Method: Post</br>
void User::add_user(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    spdlog::info("User::add_user");
    const auto request_body = req->getBody();

    auto response = util_delight::BaseResponse{}
            .set_code(k204NoContent)
            .set_message("request body is empty")
            .set_result("error");

    if (request_body.empty()) {
        callback(HttpResponse::newHttpJsonResponse(response.to_jsoncpp_json()));
        return;
    }

    auto json_value = nlohmann::json::parse(request_body);
    //判断json是否为空
    if (json_value.empty()) {
        response.set_code(k400BadRequest)
                .set_message("request body is empty");
        callback(HttpResponse::newHttpJsonResponse(response.to_jsoncpp_json()));
        return;
    }

    //检查json是否包含user_name,user_password,role
    if (!json_value.contains("user_name") || !json_value.contains("user_password") || !json_value.contains("role")) {
        response.set_code(k400BadRequest)
                .set_message("The json request body is incomplete");
        callback(HttpResponse::newHttpJsonResponse(response.to_jsoncpp_json()));
        return;
    }

    //检查user_name是否重复
    const auto user_name = json_value.at("user_name").get<std::string>();
    spdlog::info("Check user_name: {}", user_name);
    if (SqliteServiceProvider::get_instance().get_sqlite_service().check_user_exist(user_name)) {
        response.set_code(k400BadRequest)
                .set_message("The user_name already exists");
        callback(HttpResponse::newHttpJsonResponse(response.to_jsoncpp_json()));
        return;
    }

    //检查role是否为0或者1
    if (json_value.at("role") != 0 && json_value.at("role") != 1) {
        response.set_code(k400BadRequest)
                .set_message("The role must be 0 or 1");
        callback(HttpResponse::newHttpJsonResponse(response.to_jsoncpp_json()));
        return;
    }

    const auto result = SqliteServiceProvider::get_instance().get_sqlite_service().add_user(schema::BaseUser{
            .role = json_value.at("role"),
            .user_name = json_value.at("user_name").get<std::string>(),
            .password = json_value.at("user_password").get<std::string>()
    });

    if (result) {
        response.set_code(k200OK)
                .set_message("success")
                .set_result(std::to_string(result->id) + ":" + result->user_name);
        callback(HttpResponse::newHttpJsonResponse(response.to_jsoncpp_json()));
    } else {
        response.set_code(k500InternalServerError)
                .set_message("Failed to create user")
                .set_result("failed");
        callback(HttpResponse::newHttpJsonResponse(response.to_jsoncpp_json()));
    }
}

void User::find_user_by_id(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    const auto id = req->getParameter("id");
    spdlog::info("User::find_user_by_id: parm::id {}", id);

    const auto user = SqliteServiceProvider::get_instance()
            .get_sqlite_service()
            .get_user_by_id(util_delight::StringEncryption::to_number<int>(id));

    if (!user.has_value() || user->user_name.empty()) {
        callback(HttpResponse::newHttpJsonResponse(
                util_delight::BaseResponse{}
                        .set_code(k404NotFound)
                        .set_message("user not found")
                        .set_result("failed").to_jsoncpp_json()));
        return;
    }
    callback(HttpResponse::newHttpJsonResponse(
            util_delight::BaseResponse{}
                    .set_code(k200OK)
                    .set_message("success")
                    .set_result(user->user_name)
                    .to_jsoncpp_json()
    ));
}

void User::find_user_by_name(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    const auto name = req->getParameter("name");
    spdlog::info("User::find_user_by_name: parm::name {}", name);
    const auto user = SqliteServiceProvider::get_instance()
            .get_sqlite_service()
            .get_user_by_username(name);

    if (!user.has_value() || user->user_name.empty()) {
        callback(HttpResponse::newHttpJsonResponse(
                util_delight::BaseResponse{}
                        .set_code(k404NotFound)
                        .set_message("user not found")
                        .set_result("failed").to_jsoncpp_json()));
        return;
    }
    callback(HttpResponse::newHttpJsonResponse(
            util_delight::BaseResponse{}
                    .set_code(k200OK)
                    .set_message("success")
                    .set_result(nlohmann::json{
                            {"id",          user->id},
                            {"user_name",   user->user_name},
                            {"role",        user->role},
                            {"create_time", user->create_time}
                    })
                    .to_jsoncpp_json()
    ));
}
