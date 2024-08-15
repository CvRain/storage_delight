#include "api_user.h"

#include "user_response.hpp"
#include "service/sqlite_service.h"
#include "models/base_response.hpp"
#include "utils/string.h"
#include "utils/format.h"
#include "utils/response.h"
#include "nlohmann/json.hpp"

using namespace api;

void User::add_user(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
	spdlog::info("User::add_user");
	const auto request_body = req->getJsonObject();

	if (!request_body) {
		util_delight::PrettyResponse::send_base_response("request body is empty", k200OK, callback);
		return;
	}

	//判断json是否为空
	if (request_body->empty()) {
		util_delight::PrettyResponse::send_base_response("json value  is empty", k400BadRequest, callback);
		return;
	}

	//检查json是否包含user_name,user_password,role
	if (!request_body->isMember("user_name") || !request_body->isMember("user_password") || !request_body->isMember("role")) {
		util_delight::PrettyResponse::send_base_response("failed to parse json", k400BadRequest, callback);
		return;
	}

	//检查user_name是否重复
	const auto user_name = (*request_body)["user_name"].asString();
	spdlog::info("Check user_name: {}", user_name);
	if (SqliteServiceProvider::get_instance().get_sqlite_service().get_user_by_username(user_name).has_value()) {
		util_delight::PrettyResponse::send_base_response("The user_name already exists", k400BadRequest, callback);
		return;
	}


	//检查role是否为0或者1
	const auto user_role = (*request_body)["role"].asInt();
	if (user_role != 0 && user_role != 1) {
		util_delight::PrettyResponse::send_base_response("The role must be 0 or 1", k400BadRequest, callback);
		return;
	}

	//当role为0时，检查数据库中是否已经存在管理员
	if(user_role == 0 && SqliteServiceProvider::get_instance().get_sqlite_service().get_admin_count() > 0)
	{
		util_delight::PrettyResponse::send_base_response("Already exist admin account", k400BadRequest, callback);
		return;
	}


	if (const auto result = SqliteServiceProvider::get_instance().get_sqlite_service().add_user(schema::BaseUser{
		.role = user_role,
		.user_name = user_name,
		.password = (*request_body)["user_password"].asString()
	})) {
		const auto user_response = std::make_unique<model_delight::UserResponse>();
		user_response->users.emplace_back(result.value());
		user_response->m_result = "k201Created";
		user_response->m_code = k201Created;
		user_response->m_message = "Add user successfully";
		callback(HttpResponse::newHttpJsonResponse(util_delight::Format::to_jsoncpp_json(user_response->to_json())));
	}
	else {
		util_delight::PrettyResponse::send_base_response("Failed to create user", k500InternalServerError, callback);
	}
}

void User::find_user_by_id(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
	const auto id = req->getParameter("id");
	spdlog::info("User::find_user_by_id: parm::id {}", id);

	const auto user = SqliteServiceProvider::get_instance()
		.get_sqlite_service()
		.get_user_by_id(util_delight::StringEncryption::to_number<int>(id));

	if (!user.has_value() || user->user_name.empty()) {
		callback(HttpResponse::newHttpJsonResponse(
			model_delight::BaseResponse{}
			.set_code(k404NotFound)
			.set_message("user not found")
			.set_result("failed").to_json().dump()));
		return;
	}

	auto* response = new model_delight::UserResponse{};
	response->m_code = k200OK;
	response->m_message = "success";
	response->users.emplace_back(user.value());

	callback(HttpResponse::newHttpJsonResponse(
		response->to_json().dump()
	));
	delete response;
	response = nullptr;
}

void User::find_user_by_name(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
	const auto name = req->getParameter("name");
	spdlog::info("User::find_user_by_name: parm::name {}", name);
	const auto user = SqliteServiceProvider::get_instance()
		.get_sqlite_service()
		.get_user_by_username(name);

	if (!user.has_value() || user->user_name.empty()) {
		callback(HttpResponse::newHttpJsonResponse(
			model_delight::BaseResponse{}
			.set_code(k404NotFound)
			.set_message("user not found")
			.set_result("failed").to_json().dump()));
		return;
	}
	callback(HttpResponse::newHttpJsonResponse(
		model_delight::BaseResponse{}
		.set_code(k200OK)
		.set_message("success")
		.set_result(nlohmann::json{
				{"id",          user->id},
				{"user_name",   user->user_name},
				{"role",        user->role},
				{"create_time", user->create_time}
			})
		.to_json().dump()
	));
}

void User::get_user(const HttpRequest& request, std::function<void(const HttpResponse&)>&& callback) {
}
