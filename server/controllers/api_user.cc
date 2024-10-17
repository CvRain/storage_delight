#include "api_user.h"

#include "nlohmann_json_response.hpp"
#include "schema_key.hpp"
#include "utils/string.hpp"

#include "service/group_service.hpp"
#include "service/log_service.hpp"
#include "service/logger.hpp"
#include "service/user_service.hpp"

#include <iostream>

using namespace api;

void User::add_user(model_delight::NlohmannJsonRequestPtr &&req,
                    std::function<void(const HttpResponsePtr &)> &&callback) {
    service_delight::Logger::get_instance().log(service_delight::BasicLogger | service_delight::ConsoleLogger |
                                                        service_delight::DailyLogger,
                                                "Enter controller: User::add_user");

    const auto json_body = req->getNlohmannJsonBody();
    if (json_body.at(schema::key::user_role).get<int>() == schema::UserRole::TypeNone) {
        callback(model_delight::NlohmannResponse::new_common_response(&model_delight::HttpResponse{}
                                                                               .set_code(k400BadRequest)
                                                                               .set_message("Invalid role")
                                                                               .set_result("k400BadRequest")));
        return;
    }

    // 检查role是否合法
    if (const auto user_role = json_body.at(schema::key::user_role).get<int>();
        user_role < 0 || user_role >= schema::TypeRoleMax) {
        callback(model_delight::NlohmannResponse::new_common_response(&model_delight::HttpResponse{}
                                                                               .set_code(k400BadRequest)
                                                                               .set_message("Invalid role")
                                                                               .set_result("k400BadRequest")));
        return;
    }

    // 当role为0时，检查数据库中是否已经存在管理员
    if (const auto user_role = json_body.at(schema::key::user_role).get<int>();
        user_role == schema::TypeAdmin && service_delight::UserService::get_instance().admin_is_exist()) {
        callback(model_delight::NlohmannResponse::new_common_response(&model_delight::HttpResponse{}
                                                                               .set_code(k400BadRequest)
                                                                               .set_message("There is already an admin")
                                                                               .set_result("k400BadRequest")));
        return;
    }

    schema::DbUser user;
    user.name = json_body.at(schema::key::name).get<std::string>();
    user.password = util_delight::StringEncryption::sha256(json_body.at(schema::key::password).get<std::string>());
    user.role = json_body.at(schema::key::user_role).get<int>();
    user.create_time = util_delight::Date::get_current_timestamp_32();
    user.update_time = user.create_time;
    user.group_id = bsoncxx::oid{};

    nlohmann::json json_response;
    if (const auto [fst, snd] = service_delight::UserService::get_instance().add_user(&user); fst.has_value()) {
        const auto &result_user = fst.value();
        json_response[schema::key::user_id] = result_user.id.to_string();
        json_response[schema::key::name] = result_user.name;
        json_response[schema::key::user_role] = result_user.role;
        json_response[schema::key::create_time] = result_user.create_time;
        json_response[schema::key::update_time] = result_user.update_time;
        json_response[schema::key::group_id] = result_user.group_id.to_string();
    }
    else {
        nlohmann::json response{{schema::key::request::code, k400BadRequest},
                                {schema::key::request::message, "add user failed"},
                                {schema::key::request::result, snd}};
        callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));
        return;
    }

    nlohmann::json response{{schema::key::request::code, k200OK},
                            {schema::key::request::message, "add user success"},
                            {schema::key::request::result, "ok"},
                            {schema::key::request::data, json_response}};

    callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));

    schema::DbGroup group;
    group.id = user.group_id;
    group.owner_id = bsoncxx::oid{user.id};
    group.create_time = user.create_time;
    group.update_time = user.update_time;
    group.name = std::string{"group for "} + user.name;
    service_delight::GroupService::get_instance().add_group(&group);


    schema::DbOperationLog operation_log;
    operation_log.action = "add_user";
    operation_log.bucket_name = "";
    operation_log.object_name = "";
    operation_log.current_state = "";
    operation_log.description = "add user " + user.name;
    operation_log.previous_state = "";
    operation_log.user_id = user.id;
    service_delight::LogService::get_instance().record_operation(&operation_log);
}

void User::add_user_v2(model_delight::NlohmannJsonRequestPtr &&req,
                       std::function<void(const HttpResponsePtr &)> &&callback) {
    service_delight::Logger::get_instance().log(service_delight::BasicLogger | service_delight::ConsoleLogger |
                                                        service_delight::DailyLogger,
                                                "Enter controller: User::add_user");

    const auto json_body = req->getNlohmannJsonBody();

    // Check that the json body has the following key-value pairs
    const auto check_result = json_body.contains(schema::key::name) && json_body.contains(schema::key::password) &&
                              json_body.contains(schema::key::user_role);

    if (!check_result) {
        auto response = nlohmann::json{{schema::key::request::code, k400BadRequest},
                                       {schema::key::request::message, "Invalid request body"},
                                       {schema::key::request::result, "k400BadRequest"}};
        callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));
        return;
    }

    // check role is valid
    if (const auto user_role = json_body.at(schema::key::user_role).get<int>();
        user_role < 0 || user_role >= schema::TypeRoleMax) {
        auto response = nlohmann::json{{schema::key::request::code, k400BadRequest},
                                       {schema::key::request::message, "Invalid role"},
                                       {schema::key::request::result, "k400BadRequest"}};
        callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));
    }

    // check admin(role = 0) only have one
    if (const auto user_role = json_body.at(schema::key::user_role).get<int>();
        user_role == schema::TypeAdmin && service_delight::UserService::get_instance().admin_is_exist()) {
        auto response = nlohmann::json{{schema::key::request::code, k400BadRequest},
                                       {schema::key::request::message, "There is already an admin"},
                                       {schema::key::request::result, "k400BadRequest"}};
        callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));
        return;
    }

    // create user schema
    nlohmann::json user_document;
    user_document[schema::key::name] = json_body.at(schema::key::name).get<std::string>();
    user_document[schema::key::password] =
            util_delight::StringEncryption::sha256(json_body.at(schema::key::password).get<std::string>());
    user_document[schema::key::user_role] = json_body.at(schema::key::user_role).get<int>();
    user_document[schema::key::create_time] = util_delight::Date::get_current_timestamp_32();
    user_document[schema::key::update_time] = user_document[schema::key::create_time];

    // try to add user
    const auto [result, message] =
            service_delight::UserService::get_instance().add_user_v2(bsoncxx::from_json(user_document.dump()));
    if (!result.has_value()) {
        auto response = nlohmann::json{{schema::key::request::code, k500InternalServerError},
                                       {schema::key::request::message, message},
                                       {schema::key::request::result, "Failed to add user"}};
        callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));
    }

    nlohmann::json response{{schema::key::request::code, k200OK},
                            {schema::key::request::message, "add user success"},
                            {schema::key::request::result, "ok"},
                            {schema::key::request::data, result.value()}};

    callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));


    const auto user_id = result.value().at(schema::key::bson_id).get<std::string>();

    schema::DbGroup group;
    group.owner_id = bsoncxx::oid{user_id};
    group.create_time = result.value().at(schema::key::create_time).get<int32_t>();
    group.update_time = result.value().at(schema::key::update_time).get<int32_t>();
    group.name = std::string{"group for "} + result.value().at(schema::key::name).get<std::string>();
    service_delight::GroupService::get_instance().add_group(&group);


    schema::DbOperationLog operation_log;
    operation_log.action = "add_user";
    operation_log.bucket_name = "";
    operation_log.object_name = "";
    operation_log.current_state = "";
    operation_log.description = "add user " + json_body.at(schema::key::name).get<std::string>();
    operation_log.previous_state = "";
    operation_log.user_id = bsoncxx::oid{user_id};
    service_delight::LogService::get_instance().record_operation(&operation_log);
}


void User::login(model_delight::NlohmannJsonRequestPtr &&req, std::function<void(const HttpResponsePtr &)> &&callback) {

    service_delight::Logger::get_instance().log(service_delight::ConsoleLogger, "Enter controller: User::login");

    const auto client_ip = [&]() {
        if (const auto x_forward_for = req->getRequest().getHeader(schema::key::header::x_forwarded_for);
            !x_forward_for.empty()) {
            return x_forward_for;
        }
        return req->getRequest().getPeerAddr().toIp();
    }();

    service_delight::Logger::get_instance().log(service_delight::BasicLogger | service_delight::ConsoleLogger,
                                                "Login request from {}", client_ip);

    if (!req->getNlohmannJsonBody().contains(schema::key::name) ||
        !req->getNlohmannJsonBody().contains(schema::key::password)) {
        nlohmann::json response{{schema::key::request::code, k400BadRequest},
                                {schema::key::request::message, "Invalid request body"},
                                {schema::key::request::result, "k400BadRequest"}};

        service_delight::Logger::get_instance().log(service_delight::BasicLogger | service_delight::ConsoleLogger,
                                                    "Client {} failed to login: Invalid request body", client_ip);
        callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));
        return;
    };

    const auto user_name = req->getNlohmannJsonBody().at(schema::key::name).get<std::string>();
    const auto password = req->getNlohmannJsonBody().at(schema::key::password).get<std::string>();
    // 检查用户是否存在
    const auto [user_bson, result_string] = service_delight::UserService::get_instance().get_user_by_name(user_name);

    if (!user_bson.has_value()) {
        nlohmann::json response;
        response[schema::key::request::code] = k400BadRequest;
        response[schema::key::request::message] = "Failed to login";
        response[schema::key::request::result] = result_string;

        callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));
        service_delight::Logger::get_instance().log(service_delight::ConsoleLogger,
                                                    "Login failed {} : user {} not exist", client_ip, user_name);

        return;
    }

    // 检查密码是否正确
    if (const auto user_password = user_bson.value().view()[schema::key::password].get_string().value;
        util_delight::StringEncryption::sha256(password) != user_password) {
        nlohmann::json response;
        response[schema::key::request::code] = k400BadRequest;
        response[schema::key::request::message] = "Password is incorrect";
        callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));
        service_delight::Logger::get_instance().log(service_delight::ConsoleLogger,
                                                    "Login failed {} : password is incorrect", client_ip);
        return;
    }


    const auto user_id = user_bson.value().view()[schema::key::bson_id].get_oid().value.to_string();
    service_delight::Logger::get_instance().log(service_delight::ConsoleLogger, spdlog::level::debug, "Find user {}",
                                                user_id);

    const auto user_role = user_bson.value().view()[schema::key::user_role].get_int32().value;
    service_delight::Logger::get_instance().log(service_delight::ConsoleLogger, spdlog::level::debug, "Find role {}",
                                                user_role);

    // 返回jwt token
    // Json::Value header;
    // header["alg"] = "HS256";
    // header["typ"] = "JWT";
    //
    // const auto current_time = util_delight::Date::get_current_timestamp_32();
    // Json::Value payload;
    // payload["iss"] = "storage_delight";
    // payload["exp"] = current_time + 7200;
    // payload["sub"] = "login";
    // payload["iat"] = current_time;
    // payload["aud"] = user_role;
    // payload["user_id"] = user_id;
    //
    // const auto jwt = util_delight::StringEncryption::generate_jwt(header.toStyledString(), payload.toStyledString(),
    //                                                               util_delight::StringEncryption::secret_string);
    //
    // //record operation log
    // schema::DbOperationLog operation_log{};
    // operation_log.action = "login";
    // operation_log.current_state = "success";
    // operation_log.description = fmt::to_string(fmt::format("user {} login success in ip: {}", user_name, client_ip));
    // operation_log.id = bsoncxx::oid{user_id};
    // service_delight::LogService::get_instance().record_operation(&operation_log);
    //
    // Json::Value json_response{};
    // json_response["code"] = k200OK;
    // json_response["message"] = "Login successfully";
    // json_response["user_id"] = user_id;
    // json_response["user_name"] = user_name;
    // json_response["token"] = jwt;
    // callback(HttpResponse::newHttpJsonResponse(json_response));

    // 使用nlohmann::json进行重构
    nlohmann::json header{{schema::key::jwt::alg, "HS256"}, {schema::key::jwt::typ, "JWT"}};

    service_delight::Logger::get_instance().log(service_delight::ConsoleLogger, spdlog::level::debug,
                                                "User::login header: {}", header.dump());

    const auto current_time = util_delight::Date::get_current_timestamp_32();

    nlohmann::json payload{{schema::key::jwt::iss, "storage_delight"}, {schema::key::jwt::exp, current_time + 7200},
                           {schema::key::jwt::sub, "login"},           {schema::key::jwt::iat, current_time},
                           {schema::key::jwt::aud, user_role},         {schema::key::user_id, user_id}};

    service_delight::Logger::get_instance().log(service_delight::ConsoleLogger, spdlog::level::debug,
                                                "User::login payload: {}", payload.dump());

    const auto jwt = util_delight::StringEncryption::generate_jwt(header.dump(), payload.dump(),
                                                                  util_delight::StringEncryption::secret_string);

    service_delight::Logger::get_instance().log(service_delight::ConsoleLogger, spdlog::level::debug,
                                                "User::login jwt: {}", jwt);

    // record operation log
    schema::DbOperationLog operation_log{};
    operation_log.id = bsoncxx::oid{};
    operation_log.action = "login";
    operation_log.current_state = "success";
    operation_log.description = fmt::to_string(fmt::format("user {} login success in ip: {}", user_name, client_ip));
    operation_log.user_id = user_bson.value().view()[schema::key::bson_id].get_oid().value;
    service_delight::LogService::get_instance().record_operation(&operation_log);

    nlohmann::json response_part_data{
            {schema::key::user_id, user_id}, {schema::key::name, user_name}, {schema::key::token, jwt}};

    nlohmann::json response{{schema::key::request::code, k200OK},
                            {schema::key::request::message, "Login successfully"},
                            {schema::key::request::result, "ok"},
                            {schema::key::request::data, response_part_data}};

    callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));
}