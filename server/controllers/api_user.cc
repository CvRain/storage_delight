#include "api_user.h"

#include "user_response.hpp"
#include "service/sqlite_service.h"
#include "models/base_response.hpp"
#include "utils/string.h"
#include "utils/format.h"
#include "utils/response.h"
#include "nlohmann/json.hpp"

using namespace api;

void User::add_user(const HttpRequestPtr& req,
	std::function<void(const HttpResponsePtr&)>&& callback,
	schema::BaseUser&& user_request) {
	spdlog::info("User::add_user");

	if (user_request.role == schema::UserRole::TypeNone) {
		util_delight::PrettyResponse::send_base_response("response is formatted incorrectly", k400BadRequest, callback);
		return;
	}

	//检查user_name是否重复
	spdlog::info("Check user_name: {}", user_request.user_name);
	if (SqliteServiceProvider::get_instance().get_sqlite_service().get_user_by_username(
		user_request.user_name).has_value()) {
		util_delight::PrettyResponse::send_base_response("The user_name already exists", k400BadRequest, callback);
		return;
	}


	//检查role是否为0或者1
	if (user_request.role != 0 && user_request.role != 1) {
		util_delight::PrettyResponse::send_base_response("The role must be 0 or 1", k400BadRequest, callback);
		return;
	}

	//当role为0时，检查数据库中是否已经存在管理员
	if (user_request.role == 0 && SqliteServiceProvider::get_instance().get_sqlite_service().get_admin_count() > 0) {
		util_delight::PrettyResponse::send_base_response("Already exist admin account", k400BadRequest, callback);
		return;
	}


	if (const auto result = SqliteServiceProvider::get_instance()
		.get_sqlite_service()
		.add_user(schema::BaseUser{ user_request })) {
		const auto user_response = std::make_unique<model_delight::UserResponse>();
		user_response->users.emplace_back(result.value());
		user_response->m_result = "k201Created";
		user_response->m_code = k201Created;
		user_response->m_message = "Add user successfully";
		callback(HttpResponse::newHttpJsonResponse(user_response->to_json()));
	}
	else {
		util_delight::PrettyResponse::send_base_response("Failed to create user", k500InternalServerError, callback);
	}
}


void User::login(const HttpRequestPtr& req,
	std::function<void(const HttpResponsePtr&)>&& callback,
	schema::BaseUser&& user_request) {

	//检查用户是否存在
	const auto result = SqliteServiceProvider::get_instance()
		.get_sqlite_service()
		.check_user_exist(user_request.user_name);

	if (!result) {
		util_delight::PrettyResponse::send_base_response("User does not exist", k400BadRequest, callback);
		return;
	}

	const auto user = SqliteServiceProvider::get_instance()
		.get_sqlite_service()
		.get_user_by_username(user_request.user_name);


	if(util_delight::StringEncryption::sha256(user_request.password) != user.value().password)
	{
		util_delight::PrettyResponse::send_base_response("Password is incorrect", k400BadRequest, callback);
		return;
	}
	
}
