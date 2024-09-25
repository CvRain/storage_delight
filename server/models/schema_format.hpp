//
// Created by cvrain on 24-9-23.
//

#ifndef SCHEMA_FORMAT_HPP
#define SCHEMA_FORMAT_HPP

#include "db_schema.hpp"


#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <nlohmann/json.hpp>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_array;
using bsoncxx::builder::basic::make_document;

namespace util_delight {
    class SchemaFormat {
    public:
        static std::optional<schema::User> try_to_user(const nlohmann::json &user_json);
        static std::optional<schema::User> try_to_user(const bsoncxx::document::value &json_value);
        static std::optional<bsoncxx::document::value> to_bson(const schema::User &user);
    };

} // namespace util_delight

#endif
