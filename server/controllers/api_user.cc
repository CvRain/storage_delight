#include "api_user.h"

#include "nlohmann_json_response.hpp"
#include "schema_key.hpp"
#include "utils/string.hpp"

#include "service/logger.hpp"
#include "service/user_service.hpp"

using namespace api;

void User::add_user(model_delight::NlohmannJsonRequestPtr &&req, std::function<void(const HttpResponsePtr &)> &&callback) {
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

    auto user = new schema::DbUser();
    user->name = json_body.at(schema::key::name).get<std::string>();
    user->password = util_delight::StringEncryption::sha256(json_body.at(schema::key::password).get<std::string>());
    user->role = json_body.at(schema::key::user_role).get<int>();
    user->create_time = util_delight::Date::get_current_timestamp_32();
    user->update_time = user->create_time;

    nlohmann::json json_response;
    if (const auto [fst, snd] = service_delight::UserService::get_instance().add_user(user);
        fst.has_value()) {
        const auto& result_user = fst.value();
        json_response[schema::key::user_id] = result_user.id;
        json_response[schema::key::name] = result_user.name;
        json_response[schema::key::user_role] = result_user.role;
        json_response[schema::key::create_time] = result_user.create_time;
        json_response[schema::key::update_time] = result_user.update_time;
        model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(json_response));
    }
    else {
        callback(model_delight::NlohmannResponse::new_common_response(&model_delight::HttpResponse{}
                                                                               .set_code(k500InternalServerError)
                                                                               .set_message("Internal server error")
                                                                               .set_result("Failed to add user")));
    }
    delete user;
    user = nullptr;
}
//
//
// void User::login(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback,
//                  schema::BaseUser &&user_request) {
//
//     service_delight::Logger::get_instance().log(service_delight::BasicLogger | service_delight::ConsoleLogger,
//                                                 "Enter controller: User::login");
//
//     // 检查用户是否存在
//     const auto result = service_delight::UserService::get_instance().get_user_by_name(user_request.user_name);
//
//     if (!result.has_value()) {
//         callback(model_delight::NlohmannResponse::new_common_response(&model_delight::HttpResponse{}
//                                                                                .set_code(k400BadRequest)
//                                                                                .set_message("User does not exist")
//                                                                                .set_result("k400BadRequest")));
//
//         service_delight::Logger::get_instance().log(service_delight::BasicLogger | service_delight::ConsoleLogger,
//             "Login failed: {} is not exist", user_request.user_name);
//         return;
//     }
//
//     // 检查密码是否正确
//     if (util_delight::StringEncryption::sha256(user_request.password) != result.value().password) {
//         callback(model_delight::NlohmannResponse::new_common_response(&model_delight::HttpResponse{}
//                                                                                .set_code(k400BadRequest)
//                                                                                .set_message("Invalid password")
//                                                                                .set_result("k400BadRequest")));
//         service_delight::Logger::get_instance().log(service_delight::BasicLogger | service_delight::ConsoleLogger,
//             "Login failed: {} password incorrect", user_request.user_name);
//         return;
//     }
//
//     // 返回jwt token
//     Json::Value header;
//     header["alg"] = "HS256";
//     header["typ"] = "JWT";
//
//     const auto current_time = util_delight::Date::get_current_timestamp_32();
//     Json::Value payload;
//     payload["iss"] = "storage_delight";
//     payload["exp"] = current_time + 3600;
//     payload["sub"] = "login";
//     payload["iat"] = current_time;
//     payload["aud"] = result->role;
//     payload["user_id"] = result->id;
//
//     const auto jwt = util_delight::StringEncryption::generate_jwt(header.toStyledString(), payload.toStyledString(),
//                                                                   util_delight::StringEncryption::secret_string);
//
//     Json::Value json_response{};
//     json_response["code"] = k200OK;
//     json_response["message"] = "Login successfully";
//     json_response["user_id"] = result->id;
//     json_response["user_name"] = result->user_name;
//     json_response["token"] = jwt;
//     callback(HttpResponse::newHttpJsonResponse(json_response));
// }
//
// void User::get_user_by_id(model_delight::NlohmannJsonRequestPtr &&req, std::function<void(const HttpResponsePtr &)> &&callback) {
//     spdlog::info("Enter User::get_user_by_id");
//     const auto required_id = req->getRequest().getParameter("user_id");
//     if (required_id.empty()) {
//         callback(model_delight::NlohmannResponse::new_common_response(&model_delight::HttpResponse{}
//                                                                                .set_code(k400BadRequest)
//                                                                                .set_message("Invalid user_id")
//                                                                                .set_result("k400BadRequest")));
//         return;
//     }
//     const auto user = service_delight::UserService::get_instance().get_user_by_id(required_id);
//
//     const auto json_response = std::make_unique<model_delight::UserResponse>();
//     json_response->add_user(user.value())
//             .set_code(user.has_value() ? k200OK : k404NotFound)
//             .set_message(user.has_value() ? "Get user successfully" : "User does not exist")
//             .set_result(user.has_value() ? "k200OK" : "k404NotFound");
//     callback(model_delight::NlohmannResponse::new_common_response(json_response.get()));
// }
