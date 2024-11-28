//
// Created by cvrain on 24-8-19.
//

#ifndef STORAGE_DELIGHT_TYPE_HPP
#define STORAGE_DELIGHT_TYPE_HPP

#include <nlohmann/json.hpp>
#include "nlohmann_json_response.hpp"

namespace schema {
    /**
     * 定义Jwt结构体，用于表示JSON Web Token的基本结构 <br>
     * 包含token的头部和载荷部分，使用nlohmann::json进行JSON数据的封装
     */
    struct Jwt {
        nlohmann::json header;
        nlohmann::json payload;
    };

    // 定义JwtHeader结构体，用于表示Jwt的头部信息
    // 包含算法类型(alg)和token类型(typ)
    struct JwtHeader {
        std::string alg;
        std::string typ;
    };

    // 定义JwtPayload结构体，用于表示Jwt的载荷内容
    // 包含发行者(iss)、主题(sub)、受众(aud)、过期时间(exp)、发行时间(iat)和用户ID(user_id)
    struct JwtPayload {
        std::string iss;
        std::string sub;
        int         aud;
        int         exp;
        int         iat;
        std::string user_id;
    };

    // 定义JwtBody结构体，用于表示构成Jwt的完整信息
    // 包含头部信息、载荷内容以及用于验证token的密钥(secret)
    struct JwtBody {
        JwtHeader   header;
        JwtPayload  payload;
        std::string secret;
    };

    template<typename T, typename R>
    using result = std::pair<std::optional<T>, R>;
}  // namespace schema

namespace exception {
    class BaseException final : public std::exception {
    public:
        explicit                                   BaseException(const model_delight::BasicResponse& error);
        [[nodiscard]] const char*                  what() const noexcept override;
        [[nodiscard]] model_delight::BasicResponse response() const;
        void set_code(int code);
        void set_message(const std::string& message);
        void set_result(const std::string& result);
        void set_data(const nlohmann::json& data);

    private:
        model_delight::BasicResponse base_error;
    };

    inline BaseException::BaseException(const model_delight::BasicResponse& error) :
        base_error(model_delight::BasicResponse{
                .code = error.code, .message = error.message, .result = error.result, .data = error.data}) {}

    inline const char* BaseException::what() const noexcept{
        return base_error.result.data();
    }

    inline model_delight::BasicResponse BaseException::response() const{
        return base_error;
    }

    inline void BaseException::set_code(const int code){
        base_error.code = code;
    }

    inline void BaseException::set_message(const std::string& message) {
        base_error.message = message;
    }

    inline void BaseException::set_result(const std::string& result) {
        base_error.result = result;
    }


}
#endif //STORAGE_DELIGHT_TYPE_HPP
