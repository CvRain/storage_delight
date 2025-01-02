#include "api_user.h"

#include "filters/user_filter.hpp"
#include "models/basic_value.hpp"
#include "models/schema_key.hpp"
#include "nlohmann_json_response.hpp"
#include "service/group_service.hpp"
#include "service/log_service.hpp"
#include "service/logger.hpp"
#include "service/user_service.hpp"
#include "utils/exception_handler.hpp"
#include "utils/string.hpp"

using namespace api;

void User::add_user(model_delight::NlohmannJsonRequestPtr        &&req,
                    std::function<void(const HttpResponsePtr &)> &&callback) {
    service_delight::Logger::get_instance().log(
            service_delight::BasicLogger | service_delight::ConsoleLogger | service_delight::DailyLogger,
            "Enter controller: User::add_user");

    const auto json_body = req->getNlohmannJsonBody();
    if (json_body.at(schema::key::user_role).get<int32_t>() == static_cast<int32_t>(schema::UserRole::TypeNone)) {
        nlohmann::json response{{model_delight::basic_value::request::code, k400BadRequest},
                                {model_delight::basic_value::request::message, "Invalid role"},
                                {model_delight::basic_value::request::result, "k400BadRequest"},
                                {model_delight::basic_value::request::data, nlohmann::json{}}};
        callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));
        return;
    }

    // 检查role是否合法
    if (const auto user_role = json_body.at(schema::key::user_role).get<int32_t>();
        user_role < 0 or user_role >= static_cast<int32_t>(schema::UserRole::TypeRoleMax))
    {
        nlohmann::json response{{model_delight::basic_value::request::code, k400BadRequest},
                                {model_delight::basic_value::request::message, "Invalid role"},
                                {model_delight::basic_value::request::result, "k400BadRequest"},
                                {model_delight::basic_value::request::data, nlohmann::json{}}};
        callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));
        return;
    }

    // 当role为0时，检查数据库中是否已经存在管理员
    if (const auto user_role = json_body.at(schema::key::user_role).get<int>();
        user_role == static_cast<int32_t>(schema::UserRole::TypeAdmin) and
        service_delight::UserService::get_instance().admin_is_exist())
    {
        nlohmann::json response{{model_delight::basic_value::request::code, k400BadRequest},
                                {model_delight::basic_value::request::message, "There is already an admin"},
                                {model_delight::basic_value::request::result, "k400BadRequest"},
                                {model_delight::basic_value::request::data, nlohmann::json{}}};
        callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));
        return;
    }

    // 检查用户名是否存在
    if (const auto [isExist, error] = service_delight::UserService::get_instance().is_exist(
                json_body.at(schema::key::name).get<std::string>());
        not isExist.has_value() || isExist.value() == true)
    {
        nlohmann::json response{{model_delight::basic_value::request::code, k400BadRequest},
                                {model_delight::basic_value::request::message, error},
                                {model_delight::basic_value::request::result, "k400BadRequest"},
                                {model_delight::basic_value::request::data, nlohmann::json{}}};
        callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));
        return;
    }

    schema::DbUser user;
    user.name        = json_body.at(schema::key::name).get<std::string>();
    user.password    = util_delight::StringEncryption::sha256(json_body.at(schema::key::password).get<std::string>());
    user.role        = json_body.at(schema::key::user_role).get<int>();
    user.create_time = util_delight::Date::get_current_timestamp_32();
    user.update_time = user.create_time;
    user.group_id    = bsoncxx::oid{};

    auto session = service_delight::MongoProvider::get_instance().start_session();
    service_delight::MongoProvider::start_transaction(session);

    try {
        const auto &[insert_document, insert_error] =
                service_delight::UserService::get_instance().add_one(user.get_document());
        if (not insert_document.has_value()) {
            model_delight::BasicResponse response{.code    = k500InternalServerError,
                                                  .message = "k500InternalServerError",
                                                  .result  = insert_error,
                                                  .data    = {}};
            callback(newHttpJsonResponse(std::move(response.to_json())));
        }

        const auto &[get_one_document, get_one_error] = service_delight::UserService::get_instance().get_by_id(user.id);
        if (not get_one_document.has_value()) {
            service_delight::Logger::get_instance().log(service_delight::BasicLogger | service_delight::ConsoleLogger,
                                                        spdlog::level::err,
                                                        "get user failed: {}",
                                                        get_one_error);
            throw std::runtime_error(get_one_error);
        }

        const auto     user_json = schema::DbUser::to_json(get_one_document.value());
        nlohmann::json response{{model_delight::basic_value::request::code, k200OK},
                                {model_delight::basic_value::request::message, "add user success"},
                                {model_delight::basic_value::request::result, "ok"},
                                {model_delight::basic_value::request::data, user_json}};

        callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));

        schema::DbGroup group{};
        group.id          = user.group_id;
        group.owner_id    = bsoncxx::oid{user.id};
        group.create_time = user.create_time;
        group.update_time = user.update_time;
        group.name        = std::string{"group for "} + user.name;
        group.buckets     = {};
        if (const auto [group_result, group_error] =
                    service_delight::GroupService::get_instance().add_one(group.get_document());
            not group_result)
        {
            service_delight::Logger::get_instance().log(service_delight::BasicLogger | service_delight::ConsoleLogger,
                                                        spdlog::level::err,
                                                        "add group failed: {}",
                                                        group_error);
        }

        schema::DbOperationLog operation_log;
        operation_log.action      = "add_user";
        operation_log.description = "add user " + user.name;
        operation_log.user_id     = user.id;
        operation_log.request_ip  = req->getRequest().peerAddr().toIp();
        service_delight::LogService::get_instance().record_operation(&operation_log);
        service_delight::MongoProvider::commit_transaction(session);
    }
    catch (const std::exception &e) {
        service_delight::Logger::get_instance().log(
                service_delight::BasicLogger | service_delight::ConsoleLogger, "add user failed: {}", e.what());
        nlohmann::json exception_response{{model_delight::basic_value::request::code, k500InternalServerError},
                                          {model_delight::basic_value::request::message, "add user failed"},
                                          {model_delight::basic_value::request::result, e.what()},
                                          {model_delight::basic_value::request::data, nlohmann::json{}}};
        callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(exception_response)));
        service_delight::MongoProvider::abort_transaction(session);
    };
}

void User::login(model_delight::NlohmannJsonRequestPtr &&req, std::function<void(const HttpResponsePtr &)> &&callback) {
    service_delight::Logger::get_instance().log(service_delight::ConsoleLogger, "Enter controller: User::login");

    const auto client_ip = [&]() {
        if (const auto x_forward_for = req->getRequest().getHeader(model_delight::basic_value::header::x_forwarded_for);
            !x_forward_for.empty())
        {
            return x_forward_for;
        }
        return req->getRequest().getPeerAddr().toIp();
    }();

    service_delight::Logger::get_instance().log(
            service_delight::BasicLogger | service_delight::ConsoleLogger, "Login request from {}", client_ip);

    if (!req->getNlohmannJsonBody().contains(schema::key::name) ||
        !req->getNlohmannJsonBody().contains(schema::key::password))
    {
        nlohmann::json response{{model_delight::basic_value::request::code, k400BadRequest},
                                {model_delight::basic_value::request::message, "Invalid request body"},
                                {model_delight::basic_value::request::result, "k400BadRequest"}};

        service_delight::Logger::get_instance().log(service_delight::BasicLogger | service_delight::ConsoleLogger,
                                                    "Client {} failed to login: Invalid request body",
                                                    client_ip);
        callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));
        return;
    };

    const auto user_name = req->getNlohmannJsonBody().at(schema::key::name).get<std::string>();
    const auto password  = req->getNlohmannJsonBody().at(schema::key::password).get<std::string>();
    // 检查用户是否存在
    const auto [user_bson, result_string] = service_delight::UserService::get_instance().get_by_name(user_name);

    if (!user_bson.has_value()) {
        nlohmann::json response;
        response[model_delight::basic_value::request::code]    = k400BadRequest;
        response[model_delight::basic_value::request::message] = "Failed to login";
        response[model_delight::basic_value::request::result]  = result_string;

        callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));
        service_delight::Logger::get_instance().log(
                service_delight::ConsoleLogger, "Login failed {} : user {} not exist", client_ip, user_name);

        return;
    }

    // 检查密码是否正确
    if (const auto user_password = user_bson.value().view()[schema::key::password].get_string().value;
        util_delight::StringEncryption::sha256(password) != user_password)
    {
        model_delight::BasicResponse response{
                .code = k400BadRequest, .message = "Failed to login", .result = "k400BadRequest", .data = {}};
        callback(newHttpJsonResponse(response.to_json()));
        service_delight::Logger::get_instance().log(
                service_delight::ConsoleLogger, "Login failed {} : password is incorrect", client_ip);
        return;
    }


    const auto user_id = user_bson.value().view()[schema::key::bson_id].get_oid().value.to_string();
    service_delight::Logger::get_instance().log(
            service_delight::ConsoleLogger, spdlog::level::debug, "Find user {}", user_id);

    const auto user_role = user_bson.value().view()[schema::key::user_role].get_int32().value;
    service_delight::Logger::get_instance().log(
            service_delight::ConsoleLogger, spdlog::level::debug, "Find role {}", user_role);

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
    nlohmann::json header{{model_delight::basic_value::jwt::alg, "HS256"},
                          {model_delight::basic_value::jwt::typ, "JWT"}};

    service_delight::Logger::get_instance().log(
            service_delight::ConsoleLogger, spdlog::level::debug, "User::login header: {}", header.dump());

    const auto current_time = util_delight::Date::get_current_timestamp_32();

    nlohmann::json payload{{model_delight::basic_value::jwt::iss, "storage_delight"},
                           {model_delight::basic_value::jwt::exp, current_time + 72000},
                           {model_delight::basic_value::jwt::sub, "login"},
                           {model_delight::basic_value::jwt::iat, current_time},
                           {model_delight::basic_value::jwt::aud, user_role},
                           {schema::key::user_id, user_id}};

    service_delight::Logger::get_instance().log(
            service_delight::ConsoleLogger, spdlog::level::debug, "User::login payload: {}", payload.dump());

    const auto jwt = util_delight::StringEncryption::generate_jwt(
            header.dump(), payload.dump(), util_delight::StringEncryption::secret_string);

    service_delight::Logger::get_instance().log(
            service_delight::ConsoleLogger, spdlog::level::debug, "User::login jwt: {}", jwt);

    // record operation log
    schema::DbOperationLog operation_log{};
    operation_log.id            = bsoncxx::oid{};
    operation_log.action        = "login";
    operation_log.current_state = "success";
    operation_log.description   = fmt::to_string(fmt::format("user {} login success in ip: {}", user_name, client_ip));
    operation_log.user_id       = user_bson.value().view()[schema::key::bson_id].get_oid().value;
    operation_log.request_ip    = client_ip;
    service_delight::LogService::get_instance().record_operation(&operation_log);

    nlohmann::json response_part_data{
            {schema::key::user_id, user_id}, {schema::key::name, user_name}, {schema::key::token, jwt}};

    nlohmann::json response{{model_delight::basic_value::request::code, k200OK},
                            {model_delight::basic_value::request::message, "Login successfully"},
                            {model_delight::basic_value::request::result, "ok"},
                            {model_delight::basic_value::request::data, response_part_data}};

    callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));
}

void User::get_user_by_id(model_delight::NlohmannJsonRequestPtr        &&req,
                          std::function<void(const HttpResponsePtr &)> &&callback) {
    service_delight::Logger::get_instance().log(
            service_delight::ConsoleLogger, spdlog::level::debug, "User::get_user_by_id");

    try {
        const auto user_id   = req->getRequest().getParameter("user_id");
        const auto search_id = bsoncxx::oid{req->getRequest().getParameter("search_id")};

        const auto [user_info, error] = service_delight::UserService::get_instance().get_by_id(search_id);

        // 未找到用户或者搜索出现错误
        if (!user_info.has_value()) {
            service_delight::Logger::get_instance().log(
                    service_delight::ConsoleLogger, spdlog::level::warn, "User::get_user_by_id failed: {}", error);

            nlohmann::json response{{model_delight::basic_value::request::code, k500InternalServerError},
                                    {model_delight::basic_value::request::message, "User not found"},
                                    {model_delight::basic_value::request::result, error}};
            callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));

            // 记录日志：查找用户
            schema::DbOperationLog operation_log{};
            operation_log.id      = bsoncxx::oid{};
            operation_log.user_id = bsoncxx::oid{user_id};
            operation_log.action =
                    fmt::to_string(fmt::format("User::get_user_by_id search: {}", search_id.to_string()));
            operation_log.current_state = "failed";
            operation_log.description =
                    fmt::to_string(fmt::format("user {} failed to get user by id: {}", user_id, error));
            operation_log.request_ip = req->getRequest().getPeerAddr().toIp();
            service_delight::LogService::get_instance().record_operation(&operation_log);
            return;
        }

        nlohmann::json response{
                {model_delight::basic_value::request::code, k200OK},
                {model_delight::basic_value::request::message, "User found"},
                {model_delight::basic_value::request::result, "ok"},
                {model_delight::basic_value::request::data, schema::DbUser::to_json(user_info.value())}};
        callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));

        schema::DbOperationLog operation_log{};
        operation_log.id      = bsoncxx::oid{};
        operation_log.user_id = bsoncxx::oid{user_id};
        operation_log.action  = fmt::to_string(fmt::format("User::get_user_by_id search: {}", search_id.to_string()));
        operation_log.current_state = "success";
        operation_log.description =
                fmt::to_string(fmt::format("user {} get user by id: {}", user_id, search_id.to_string()));
        operation_log.request_ip = req->getRequest().getPeerAddr().toIp();
        service_delight::LogService::get_instance().record_operation(&operation_log);
    }
    catch (const std::exception &e) {
        service_delight::Logger::get_instance().log(
                service_delight::ConsoleLogger, spdlog::level::warn, "User::get_user_by_id failed: {}", e.what());
        nlohmann::json response{{model_delight::basic_value::request::code, k500InternalServerError},
                                {model_delight::basic_value::request::message, "Internal server error"},
                                {model_delight::basic_value::request::result, e.what()}};
        callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));
    }
}

void User::user_info(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    service_delight::Logger::get_instance().log(
            service_delight::ConsoleLogger, spdlog::level::debug, "User::user_info");
    try {
        const auto request_body       = fromRequest<nlohmann::json>(*req);
        const auto search_id          = bsoncxx::oid{request_body.at("search_id").get<std::string>()};
        const auto [user_info, error] = service_delight::UserService::get_instance().get_by_id(search_id);
        if (!user_info.has_value()) {
            service_delight::Logger::get_instance().log(
                    service_delight::ConsoleLogger, spdlog::level::warn, "User::user_info failed: {}", error);
            nlohmann::json response{{model_delight::basic_value::request::code, k500InternalServerError},
                                    {model_delight::basic_value::request::message, "User not found"},
                                    {model_delight::basic_value::request::result, error}};
            callback(newHttpJsonResponse(std::move(response)));
        }
        const auto                  &user_value = schema::DbUser::to_json(user_info.value());
        model_delight::BasicResponse response{.code = k200OK, .message = "k200OK", .result = "ok", .data = user_value};
        callback(newHttpJsonResponse(response.to_json()));
    }
    catch (const std::exception &exception) {
        exception::ExceptionHandler::handle(req, std::move(callback), exception);
    }
}

void User::all_user_info(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    service_delight::Logger::get_instance().log(
            service_delight::ConsoleLogger, spdlog::level::debug, "User::all_user_info");
    try {
        const auto [user_info, error] = service_delight::UserService::get_instance().list_users();
        if (!user_info.has_value()) {
            service_delight::Logger::get_instance().log(
                    service_delight::ConsoleLogger, spdlog::level::warn, "User::all_user_info failed: {}", error);
            nlohmann::json response{{model_delight::basic_value::request::code, k500InternalServerError},
                                    {model_delight::basic_value::request::message, "User not found"},
                                    {model_delight::basic_value::request::result, error}};
        }
        nlohmann::json user_info_array{};
        for (const auto &it: user_info.value()) {
            user_info_array.push_back(nlohmann::json::parse(bsoncxx::to_json(it)));
        }
        model_delight::BasicResponse response{
                .code = k200OK, .message = "k200OK", .result = "ok", .data = std::move(user_info_array)};
        callback(newHttpJsonResponse(response.to_json()));
    }
    catch (const std::exception &exception) {
        exception::ExceptionHandler::handle(req, std::move(callback), exception);
    }
}
void User::remove_user(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    service_delight::Logger::get_instance().log(
            service_delight::ConsoleLogger, spdlog::level::debug, "User::remove_user");
    try {
        const auto request_body = fromRequest<nlohmann::json>(*req);
        const auto user_id      = bsoncxx::oid{request_body.at("user_id").get<std::string>()};
        // remove user
        const auto [user_info, error] = service_delight::UserService::get_instance().remove_one(user_id);
        if (!user_info.has_value()) {
            service_delight::Logger::get_instance().log(
                    service_delight::ConsoleLogger, spdlog::level::warn, "User::remove_user failed: {}", error);
            nlohmann::json response{{model_delight::basic_value::request::code, k500InternalServerError},
                                    {model_delight::basic_value::request::message, "User not found"},
                                    {model_delight::basic_value::request::result, error}};
            callback(newHttpJsonResponse(std::move(response)));
        }

        // remove user's group
        const auto [group_info, remove_error] = service_delight::GroupService::get_instance().remove_by_owner(user_id);
        if (const auto value = group_info.value_or(false); !value) {
            service_delight::Logger::get_instance().log(
                    service_delight::ConsoleLogger, spdlog::level::warn, "User::remove_user failed: {}", remove_error);
            nlohmann::json response{{model_delight::basic_value::request::code, k500InternalServerError},
                                    {model_delight::basic_value::request::message, "User not found"},
                                    {model_delight::basic_value::request::result, remove_error}};
            callback(newHttpJsonResponse(std::move(response)));
        }

        model_delight::BasicResponse response{.code = k200OK, .message = "k200OK", .result = "ok"};
        callback(newHttpJsonResponse(response.to_json()));
        // todo record operation
    }
    catch (const std::exception &exception) {
        exception::ExceptionHandler::handle(req, std::move(callback), exception);
    }
}

/// <summary>
/// 接受一串用户id并且返回对应的用户名
/// 接收
/// <code type="json">
/// {"user_ids": [
///  "xxx", "xxx"
/// ]}
/// </code>
/// 返回
/// <code type="json">
/// {"user_names": [
///     {{"id":"xxx},{"name":"xxx"}},
///     {{"id":"xxx},{"name":"xxx"}}
/// ]}
/// </code>
/// </summary>
void User::get_user_name(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    service_delight::Logger::get_instance().log(
            service_delight::ConsoleLogger, spdlog::level::debug, "User::get_user_name");
    try {
        const auto                  request_body = fromRequest<nlohmann::json>(*req);
        const auto                  user_ids     = request_body.at("user_ids").get<std::vector<std::string>>();
        std::vector<nlohmann::json> user_names{};
        for (const auto &user_id: user_ids) {
            const auto [user_info, error] =
                    service_delight::UserService::get_instance().get_by_id(bsoncxx::oid{user_id});
            if (!user_info.has_value()) {
                service_delight::Logger::get_instance().log(
                        service_delight::ConsoleLogger, spdlog::level::warn, "User::get_user_name failed: {}", error);
                nlohmann::json response{{model_delight::basic_value::request::code, k500InternalServerError},
                                        {model_delight::basic_value::request::message, "User not found"},
                                        {model_delight::basic_value::request::result, error}};
                return callback(newHttpJsonResponse(std::move(response)));
            }
            const auto &name = user_info.value().find("name")->get_string();
            user_names.push_back(nlohmann::json{{"id", user_id}, {"name", name}});
        }
        model_delight::BasicResponse response{
                .code = k200OK, .message = "k200OK", .result = "ok", .data = std::move(user_names)};
        callback(newHttpJsonResponse(response.to_json()));
    }
    catch (const std::exception &exception) {
        exception::ExceptionHandler::handle(req, std::move(callback), exception);
    }
}

void User::is_admin_account(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback){
    service_delight::Logger::get_instance().log(
            service_delight::ConsoleLogger, spdlog::level::debug, "User::is_admin_account");
    try {
        const auto request_body = fromRequest<nlohmann::json>(*req);
        const auto user_id      = bsoncxx::oid{request_body.at("user_id").get<std::string>()};
        const auto is_admin_result = service_delight::UserService::get_instance().is_admin(user_id);
        model_delight::BasicResponse response{
        .code = is_admin_result ? k200OK : k404NotFound,
        .message = is_admin_result ? "k200OK" : "k404NotFound",
        .result = is_admin_result ? "is admin" : "not admin",
        .data = {}};
        callback(newHttpJsonResponse(response.to_json()));
    }
    catch (const std::exception &exception) {
        exception::ExceptionHandler::handle(req, std::move(callback), exception);
    }
}