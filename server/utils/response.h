#ifndef RESPONSE_H
#define RESPONSE_H

#include <json/json.h>
#include <nlohmann/json.hpp>
#include <string>
#include <concepts>
#include <type_traits>

namespace util_delight
{
    class BaseResponse{
    public:
        std::string m_result;
        std::string m_message;
        int m_code;

        BaseResponse& set_result(const std::string& result){
            m_result = result;
            return *this;
        }

        BaseResponse& set_message(const std::string& message){
            m_message = message;
            return *this;
        }

        BaseResponse& set_code(int code){
            m_code = code;
            return *this;
        }

        virtual nlohmann::json to_nlohmann_json(){
            return nlohmann::json {
                {"result", m_result},
                {"message", m_message},
                {"code", m_code}
            };
        }

        virtual Json::Value to_jsoncpp_json(){
            Json::Value json;
            json["result"] = m_result;
            json["message"] = m_message;
            json["code"] = m_code;
            
            return json;
        }
        
    };
} // namespace utils


#endif