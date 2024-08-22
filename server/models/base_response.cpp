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

    CommonResponse &CommonResponse::append(std::map<std::string, Json::Value> &&map) {
        for(const auto& [key, value] : map){
            json_root[key] = value;
        };
        return *this;
    }

    CommonResponse &CommonResponse::append(Json::Value &&value) {
        json_root.append(std::move(value));
        return *this;
    }

    CommonResponse &CommonResponse::append(std::string &&key, Json::Value &&value) {
        json_root[key] = std::move(value);
        return *this;
    }

    Json::Value CommonResponse::to_json() {

        return json_root;
    }
}
