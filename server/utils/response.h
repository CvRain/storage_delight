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
        std::string result;
        std::string message;
        uint8_t code;

        BaseResponse& set_result(std::string result){
            this->result = result;
            return *this;
        }

        BaseResponse& set_message(std::string message){
            this->message = message;
            return *this;
        }

        BaseResponse& set_code(uint8_t code){
            this->code = code;
            return *this;
        }

        template<typename T>
        T to_json(){
            if(std::is_same_v<T, Json::Value>){
                return to_jsoncpp_json();
            }
            else if(std::is_same_v<T, nlohmann::json>){
                return to_nlohmann_json();
            }
            else{
                throw std::runtime_error("Invalid type");
            }
        }
        
        virtual nlohmann::json to_nlohmann_json(){
            return nlohmann::json {
                {"result", result},
                {"message", message},
                {"code", code}
            };
        }

        virtual Json::Value to_jsoncpp_json(){
            Json::Value json;
            json["result"] = result;
            json["message"] = message;
            json["code"] = code;
            
            return json;
        }
        
    };
} // namespace utils


#endif