//
// Created by cvrain on 24-10-5.
//

#include "group_service.hpp"

#include "logger.hpp"
#include "schema_key.hpp"

namespace service_delight {
    void GroupService::init() {
        Logger::get_instance().log(BasicLogger | ConsoleLogger, "Enter GroupService::init");
        group_collection = MongoService::get_instance().get_collection(schema::key::collection::group);
    }

    schema::result<schema::DbUser, std::string> GroupService::add_group(schema::DbGroup &group) {
        Logger::get_instance().log(BasicLogger | ConsoleLogger, "Enter GroupService::add_group");
        //check group exist
        if (group_exist(group.id).first) {
            return std::make_pair(std::nullopt, "Group already exist");
        }
        try {
            const auto result = group_collection.insert_one(group.get_document());
            return std::make_pair(group, "");
        }catch (const std::exception &e) {
            Logger::get_instance().log(BasicLogger | ConsoleLogger,
                                       "Exception in GroupService::add_group: %s", e.what());
            return std::make_pair(std::nullopt, e.what());
        }
    }

    schema::result<schema::DbGroup, std::string> GroupService::get_group(const std::string &group_id) {
        Logger::get_instance().log(BasicLogger | ConsoleLogger, "Enter GroupService::get_group");
        const auto bson_group_id = bsoncxx::oid{group_id};
        try {
            const auto result = group_collection.find_one(make_document(
                    kvp(schema::key::bson_id, bson_group_id)
                    ));
            if (result.has_value()) {
                const auto &db_group = result.value();
                const auto group = schema::DbGroup::from_bson(db_group);
                return std::make_pair(group, "");
            }
            return std::make_pair(std::nullopt, "Group not found");
        }
        catch (const std::exception &e) {
            Logger::get_instance().log(BasicLogger | ConsoleLogger,
                                       "Exception in GroupService::get_group: %s", e.what());
            return std::make_pair(std::nullopt, e.what());
        }
    }

    schema::result<bool, std::string> GroupService::group_exist(const std::string &group_id) {
        Logger::get_instance().log(BasicLogger | ConsoleLogger, "Enter GroupService::group_exist");
        const auto bson_group_id = bsoncxx::oid{group_id};
        try {
            const auto result = group_collection.find_one(make_document(
                    kvp(schema::key::bson_id, bson_group_id)
                    ));
            return std::make_pair(result.has_value(), "");
        }
        catch (const std::exception &e) {
            Logger::get_instance().log(BasicLogger | ConsoleLogger,
                                       "Exception in GroupService::group_exist: %s", e.what());
            return std::make_pair(false, e.what());
        }
    }
} // service_delightz
