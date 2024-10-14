//
// Created by cvrain on 24-9-22.
//

#include "mongo_service.hpp"
#include "service/logger.hpp"

namespace service_delight {
    MongoService::MongoService(const nlohmann::json &config) :
        pool(mongocxx::uri{config.at("mongodb").at("url").get<std::string>()}) {

        Logger::get_instance().log(ConsoleLogger | BasicLogger, "MongoService::MongoService");

        // list of collections
        try {
            const auto client = pool.acquire();
            for (auto db = client.operator*()[schema::key::database::db_name];
                 const auto& collection: db.list_collection_names()) {
                Logger::get_instance().log(ConsoleLogger | BasicLogger, "Find collection: {}", collection);
            }
        }
        catch (const std::exception &e) {
            Logger::get_instance().log(ConsoleLogger | BasicLogger, spdlog::level::trace, "Error listing collections: {}", e.what());
        }
    }


    mongocxx::collection MongoService::get_collection(const std::string &collection_name) {
        const auto client = pool.acquire();
        return client.operator*()[schema::key::database::db_name][collection_name];
    }

    void MongoProvider::init(const nlohmann::json &json) {
        Logger::get_instance().log(ConsoleLogger | BasicLogger, "MongoProvider::init");
        mongo_service = std::make_unique<MongoService>(json);
    }

    mongocxx::collection MongoProvider::get_collection(const std::string &collection_name) const {
        return mongo_service->get_collection(collection_name);
    }


} // namespace service_delight
