#pragma once

#include <spdlog/spdlog.h>
#include <string>
#include <nlohmann/json.hpp>
#include <json/json.h>

namespace util_delight
{
	class Format {
	public:
		static Json::Value to_jsoncpp_json(const nlohmann::json& json);
	};

	inline Json::Value Format::to_jsoncpp_json(const nlohmann::json& json) {
		const auto json_str = json.dump();

		Json::Value json_cpp_value{};
		const Json::CharReaderBuilder builder;

		std::istringstream json_str_stream{ json_str };
		std::string errors{};

		if (const auto result = Json::parseFromStream(builder, json_str_stream, &json_cpp_value, &errors); !result) {
			spdlog::error("Failed to parse json: {}", errors);
		}

		return json_cpp_value;
	}
}
