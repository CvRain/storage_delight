#pragma once

#include "base_response.hpp"
#include "db_schema.hpp"

#include <vector>
#include <json/json.h>

namespace model_delight
{
	class UserResponse final : public BaseResponse
	{
	public:
		UserResponse& set_users(std::vector<schema::User>&& source_users);

		UserResponse& add_user(const schema::User& user);

		Json::Value to_json() override;

	public:
		std::vector<schema::User> users;
	};
}
