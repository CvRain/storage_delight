#ifndef RESPONSE_H
#define RESPONSE_H

#include <string>
#include <json/json.h>

namespace model_delight
{
	class BaseResponse
	{
	public:
		virtual ~BaseResponse();

		BaseResponse& set_result(const std::string& result);

		BaseResponse& set_message(const std::string& message);

		BaseResponse& set_code(int code);

		virtual Json::Value to_json();

	public:
		std::string m_result{};

		std::string m_message{};

		int m_code = 200;
	};
} // namespace utils


#endif
