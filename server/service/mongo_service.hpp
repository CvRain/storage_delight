//
// 单例模式的Mongodb Server, 用于获取数据库连接，返回collection
//

#ifndef MONGO_SERVICE_HPP
#define MONGO_SERVICE_HPP

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>

#include <nlohmann/json.hpp>

#include "utils/singleton_prototype.hpp"

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_array;
using bsoncxx::builder::basic::make_document;

namespace service_delight {
    class MongoService final: public util_delight::Singleton<MongoService> {
    public:
        ~MongoService() = default;

        void init(const nlohmann::json &config);

        [[nodiscard]] mongocxx::collection get_collection(const std::string &collection_name) const;
    private:
        mongocxx::client client;
        mongocxx::database database;
    };
} // service_delight

#endif //MONGO_SERVICE_HPP
