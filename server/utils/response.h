#pragma once

#include <string>
#include <functional>
#include <drogon/HttpResponse.h>

#include "models/base_response.hpp"
#include "utils/format.h"

namespace util_delight {
	class PrettyResponse {
	public:
		static void send_base_response(const std::string& message, const int code,
			const std::function<void(const HttpResponsePtr&)>& callback);
	};

	inline void PrettyResponse::send_base_response(const std::string& message, const int code,
		const std::function<void(const HttpResponsePtr&)>& callback) {
		const auto response = model_delight::BaseResponse{}
			.set_code(code)
			.set_message(message)
			.to_json();
		callback(HttpResponse::newHttpJsonResponse(response));
	}
}