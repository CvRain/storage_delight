//
// Created by cvrain on 24-10-8.
//

#include "log_service.hpp"

#include "service/mongo_service.hpp"

namespace service_delight {
    void LogService::init() {
        log_collection = MongoProvider::get_instance().get_collection(schema::key::collection::operation_logs);
    }
    void LogService::record_operation(schema::DbOperationLog *operation_log) {
        if (const auto result = log_collection.insert_one(operation_log->get_document().view()); !result.has_value()) {
            const auto json_result = bsoncxx::to_json(operation_log->get_document().view());
            Logger::get_instance().log(ConsoleLogger, spdlog::level::err, "record operation log failed: {}", json_result.data());
        }
    }

    void LogService::record_operation(const nlohmann::json &operation_log){
        auto document = bsoncxx::from_json(operation_log.dump());
        if(const auto result = log_collection.insert_one(document.view()); !result.has_value()){
            Logger::get_instance().log(ConsoleLogger, spdlog::level::err, "record operation log failed: {}", operation_log.dump());
        }
    }

    schema::result<nlohmann::json, std::string> LogService::get_operation_by_id(const bsoncxx::oid &record_id) {
        const auto result =
                log_collection.find_one(make_document(kvp(schema::key::bson_id, record_id)));
        if (!result.has_value()) {
            return std::make_pair(std::nullopt, "get operation log failed");
        }
        const auto document = bsoncxx::to_json(result.value().view());
        return std::make_pair(nlohmann::json::parse(document), "");
    }

    schema::result<nlohmann::json, std::string> LogService::get_operation_by_user_id(const bsoncxx::oid &user_id) {
        const auto result = log_collection.find_one(make_document(kvp(schema::key::user_id, user_id)));
        if (!result.has_value()) {
            return std::make_pair(std::nullopt, "get operation log failed");
        }
        const auto document = bsoncxx::to_json(result.value().view());
        return std::make_pair(nlohmann::json::parse(document), "");
    }
} // service_delight