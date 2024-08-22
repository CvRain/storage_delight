//
// Created by cvrain on 24-8-19.
//

#ifndef STORAGE_DELIGHT_TYPE_HPP
#define STORAGE_DELIGHT_TYPE_HPP

#include <nlohmann/json.hpp>

namespace schema{
    struct Jwt{
        nlohmann::json header;
        nlohmann::json payload;
    };

    struct JwtHeader{
        std::string alg;
        std::string typ;
    };

    struct JwtPayload{
        std::string iss;
        std::string sub;
        int aud;
        int exp;
        int iat;
        int user_id;
    };

    struct JwtBody{
        JwtHeader header;
        JwtPayload payload;
        std::string secret;
    };
}

#endif //STORAGE_DELIGHT_TYPE_HPP
