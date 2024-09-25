//
// Created by cvrain on 24-9-22.
//

#include "mongo_service.hpp"
#include "service/logger.hpp"

namespace service_delight {
    /*
     * "mongodb": {
     *  "url": "mongodb://localhost:27017",
     *  "db_name": "storage_delight"
     * }
     */
    void MongoService::init(const nlohmann::json &config) {
        mongocxx::instance instance{};
        const auto& json_root = config.at("mongodb");
        const mongocxx::uri url{json_root.at("url").get<std::string>()};

        Logger::get_instance().log(BasicLogger | ConsoleLogger | DailyLogger,
                                   "MongoDB client connected to {}", url.to_string());

        const auto db_name = json_root.at("db_name").get<std::string>();
        client = mongocxx::client{url};
        database = client[db_name];

        const auto collection_name = database.list_collection_names();
        for (const auto& name : collection_name) {
            Logger::get_instance().log(BasicLogger | ConsoleLogger | DailyLogger,
                                       "Load MongoDB collection: {}", name);
        }
    }

    mongocxx::collection MongoService::get_collection(const std::string &collection_name) const {
        return database[collection_name];
    }
} // service_delight
