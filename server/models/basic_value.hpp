//
// Created by cvrain on 24-9-23.
//

#ifndef BASIC_VALUE_HPP
#define BASIC_VALUE_HPP

#include <string>
#include <string_view>

namespace model_delight::basic_value {
    namespace request{
        const std::string status = "status";
        const std::string message = "message";
        const std::string data = "data";
        const std::string code = "code";
        const std::string error = "error";
        const std::string success = "success";
        const std::string result = "result";
        const std::string total = "total";
        const std::string page = "page";
        const std::string page_size = "page_size";
    }

    namespace header{
        const std::string authorization = "Authorization";
        const std::string content_type = "Content-Type";
        const std::string content_length = "Content-Length";
        const std::string content_disposition = "Content-Disposition";
        const std::string content_encoding = "Content-Encoding";
        const std::string content_range = "Content-Range";
        const std::string content_md5 = "Content-MD5";
        const std::string content_type_json = "application/json";
        const std::string content_type_form_data = "multipart/form-data";
        const std::string content_type_text_plain = "text/plain";
        const std::string content_type_text_html = "text/html";
        const std::string x_forwarded_for = "X-Forwarded-For";
    }

    namespace jwt {
        constexpr std::string_view alg = "alg";
        constexpr std::string_view typ = "typ";
        constexpr std::string_view kid = "kid";
        constexpr std::string_view iss = "iss";
        constexpr std::string_view sub = "sub";
        constexpr std::string_view aud = "aud";
        constexpr std::string_view exp = "exp";
        constexpr std::string_view nbf = "nbf";
        constexpr std::string_view iat = "iat";
        constexpr std::string_view jti = "jti";
    }

    namespace middleware {
        const std::string LoginAuth ="drogon::middleware::LoginMiddleware";
        const std::string AdminAuth = "drogon::middleware::AdminAuth";
    }
}


#endif // BASIC_VALUE_HPP
