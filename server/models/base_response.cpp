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

	Json::Value BaseResponse::to_json()
	{
        Json::Value root;
        root["result"] = m_result;
        root["message"] = m_message;
        root["code"] = m_code;
        return root;
	}
}
