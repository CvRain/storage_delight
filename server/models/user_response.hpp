#pragma once

#include "http_response.hpp"
#include "db_schema.hpp"

#include <vector>


namespace model_delight
{
	class UserResponse final : public HttpResponse
	{
	public:
		UserResponse& set_users(std::vector<schema::User>&& source_users);

		UserResponse& add_user(const schema::User& user);

		nlohmann::json to_json() override;

		std::vector<schema::User> users;
	};
}