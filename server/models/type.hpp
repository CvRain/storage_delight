//
// Created by cvrain on 24-8-19.
//

#ifndef STORAGE_DELIGHT_TYPE_HPP
#define STORAGE_DELIGHT_TYPE_HPP

#include <nlohmann/json.hpp>

namespace schema{
    /**
     * 定义Jwt结构体，用于表示JSON Web Token的基本结构 <br>
     * 包含token的头部和载荷部分，使用nlohmann::json进行JSON数据的封装
     */
     struct Jwt{
        nlohmann::json header;
        nlohmann::json payload;
    };

    // 定义JwtHeader结构体，用于表示Jwt的头部信息
    // 包含算法类型(alg)和token类型(typ)
    struct JwtHeader{
        std::string alg;
        std::string typ;
    };

    // 定义JwtPayload结构体，用于表示Jwt的载荷内容
    // 包含发行者(iss)、主题(sub)、受众(aud)、过期时间(exp)、发行时间(iat)和用户ID(user_id)
    struct JwtPayload{
        std::string iss;
        std::string sub;
        int aud;
        int exp;
        int iat;
        std::string user_id;
    };

    // 定义JwtBody结构体，用于表示构成Jwt的完整信息
    // 包含头部信息、载荷内容以及用于验证token的密钥(secret)
    struct JwtBody{
        JwtHeader header;
        JwtPayload payload;
        std::string secret;
    };

    template<typename T, typename R>
    using result = std::pair<std::optional<T>, R>;
}

#endif //STORAGE_DELIGHT_TYPE_HPP
