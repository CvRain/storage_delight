#include "base_response.hpp"

namespace model_delight{
	BaseResponse::~BaseResponse()
	= default;

	BaseResponse& BaseResponse::set_result(const std::string& result)
	{
		m_result = result;
		return *this;
	}

	BaseResponse& BaseResponse::set_message(const std::string& message)
	{
		m_message = message;
		return *this;
	}

	BaseResponse& BaseResponse::set_code(const int code)
	{
		m_code = code;
		return *this;
	}

	nlohmann::json BaseResponse::to_json()
	{
		return nlohmann::json{
			{"result", m_result},
			{"message", m_message},
			{"code", m_code}
		};
	}
}
