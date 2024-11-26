//
// Created by cvrain on 24-9-23.
//

#ifndef BASIC_VALUE_HPP
#define BASIC_VALUE_HPP

#include <string>
#include <string_view>

namespace model_delight::basic_value {
    namespace request {
        const std::string status    = "status";
        const std::string message   = "message";
        const std::string data      = "data";
        const std::string code      = "code";
        const std::string error     = "error";
        const std::string success   = "success";
        const std::string result    = "result";
        const std::string total     = "total";
        const std::string page      = "page";
        const std::string page_size = "page_size";
    }  // namespace request

    namespace header {
        const std::string authorization           = "Authorization";
        const std::string content_type            = "Content-Type";
        const std::string content_length          = "Content-Length";
        const std::string content_disposition     = "Content-Disposition";
        const std::string content_encoding        = "Content-Encoding";
        const std::string content_range           = "Content-Range";
        const std::string content_md5             = "Content-MD5";
        const std::string content_type_json       = "application/json";
        const std::string content_type_form_data  = "multipart/form-data";
        const std::string content_type_text_plain = "text/plain";
        const std::string content_type_text_html  = "text/html";
        const std::string x_forwarded_for         = "X-Forwarded-For";
    }  // namespace header

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
    }  // namespace jwt

    namespace middleware {
        constexpr auto LoginAuth  = "drogon::middleware::LoginMiddleware";
        constexpr auto AdminAuth  = "drogon::middleware::AdminAuth";
        constexpr auto UserExist  = "drogon::middleware::UserExist";
        constexpr auto GroupExist = "drogon::middleware::GroupExist";
        constexpr auto SourceExist = "drogon::middleware::SourceExist";

    }  // namespace middleware
}  // namespace model_delight::basic_value

/**
 * bson查询关键字操作符 <br>
 * <a href="https://www.mongodb.com/zh-cn/docs/manual/reference/operator/query/"> query: 查询与投影操作符</a> <br>
 * <a href="https://www.mongodb.com/zh-cn/docs/manual/reference/operator/update/"> update: 更新操作符</a>
 */
namespace model_delight::bson_operator {
    namespace query {
        // 比较操作符
        constexpr std::string_view eq  = "$eq";  // 匹配等于指定值的值。
        constexpr std::string_view gt  = "$gt";  // 匹配大于指定值的值。
        constexpr std::string_view gte = "$gte";  // 匹配大于等于指定值的值。
        constexpr std::string_view in  = "$in";  // 匹配数组中指定的任何值。
        constexpr std::string_view lt  = "$lt";  // 匹配小于指定值的值。
        constexpr std::string_view lte = "$lte";  // 匹配小于等于指定值的值。
        constexpr std::string_view ne  = "$ne";  // 匹配所有不等于指定值的值。
        constexpr std::string_view nin = "$nin";  // 不匹配数组中指定的任何值。
        // 逻辑操作符
        constexpr std::string_view and_ = "$and";  // 使用逻辑 AND 连接查询子句将返回与两个子句的条件匹配的所有文档。
        constexpr std::string_view not_ = "$not";  // 反转查询谓词的效果，并返回与查询谓词不匹配的文档。
        constexpr std::string_view or_  = "$or";  // 使用逻辑 NOR 的联接查询子句会返回无法匹配这两个子句的所有文档。
        constexpr std::string_view nor_ = "$nor";  // 使用逻辑 OR 连接多个查询子句会返回符合任一子句条件的所有文档。
        // 元素操作符
        constexpr std::string_view exists = "$exists";  // 匹配具有指定字段的文档。
        constexpr std::string_view type   = "$type";  // 如果字段为指定类型，则选择文档。
        // 求值操作
        constexpr std::string_view expr        = "$expr";
        constexpr std::string_view json_schema = "$jsonSchema";
        constexpr std::string_view mod         = "$mod";
        constexpr std::string_view regex       = "$regex";
        constexpr std::string_view text        = "$text";
        constexpr std::string_view where       = "$where";
        // 地理空间
        constexpr std::string_view geo_intersects = "$geoIntersects";
        constexpr std::string_view geo_within     = "$geoWithin";
        constexpr std::string_view near           = "$near";
        constexpr std::string_view near_sphere    = "$nearSphere";
        // 阵列
        constexpr std::string_view all        = "$all";
        constexpr std::string_view elem_match = "$elemMatch";
        constexpr std::string_view size       = "$size";
        // Bitwise
        constexpr std::string_view bits_all_clear = "$bitsAllClear";
        constexpr std::string_view bits_all_set   = "$bitsAllSet";
        constexpr std::string_view bits_any_clear = "$bitsAnyClear";
        constexpr std::string_view bits_any_set   = "$bitsAnySet";
        // 投影操作符
        constexpr std::string_view first = "$";
        // constexpr std::string_view elem_match = "$elemMatch";
        constexpr std::string_view mata  = "$meta";
        constexpr std::string_view slice = "$slice";
        // 其他操作符
        constexpr std::string_view comment = "$comment";
        constexpr std::string_view rand    = "$rand";

    }  // namespace query

    namespace update {
        namespace field {
            constexpr std::string_view current_date  = "$currentDate";
            constexpr std::string_view inc           = "$inc";
            constexpr std::string_view min           = "$min";
            constexpr std::string_view max           = "$max";
            constexpr std::string_view mul           = "$mul";
            constexpr std::string_view rename        = "$rename";
            constexpr std::string_view set           = "$set";
            constexpr std::string_view set_on_insert = "$setOnInsert";
            constexpr std::string_view unset         = "$unset";
        }  // namespace field

        namespace array {
            constexpr std::string_view first      = "$";
            constexpr std::string_view all        = "$[]";
            constexpr std::string_view add_to_set = "$addToSet";
            constexpr std::string_view pop        = "$pop";
            constexpr std::string_view pull = "$pull";
            constexpr std::string_view push = "$push";
            constexpr std::string_view pull_all = "$pullAll";
        }

        namespace modifiers {
            constexpr std::string_view each = "$each";
            constexpr std::string_view position = "$position";
            constexpr std::string_view slice = "$slice";
            constexpr std::string_view sort = "$sort";
        }

        namespace bitwise {
            constexpr std::string_view bit = "$bit";
        }
    }
}


#endif // BASIC_VALUE_HPP
