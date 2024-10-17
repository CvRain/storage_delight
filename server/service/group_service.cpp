//
// Created by cvrain on 24-10-5.
//

#include "group_service.hpp"

#include "logger.hpp"
#include "schema_key.hpp"

namespace service_delight {
    void GroupService::init() {
        Logger::get_instance().log(BasicLogger | ConsoleLogger, "Enter GroupService::init");
        group_collection = MongoProvider::get_instance().get_collection(schema::key::collection::group);
    }

    auto GroupService::add_group(schema::DbGroup *group) -> schema::result<std::string, std::string> {
        Logger::get_instance().log(BasicLogger | ConsoleLogger, "Enter GroupService::add_group");

        // check group exist
        if (const auto result = group_exist(group->id).first; result == true) {
            Logger::get_instance().log(BasicLogger | ConsoleLogger, "Leave GroupService::add_group");
            return std::make_pair(std::nullopt, "Group already exist");
        }
        try {
            const auto result = group_collection.insert_one(group->get_document().view());
            if (!result.has_value()) {
                Logger::get_instance().log(BasicLogger | ConsoleLogger, "GroupService::add_group failed");
                return std::make_pair(std::nullopt, "GroupService::add_group failed");
            }
            const auto id = result.value().inserted_id().get_string().value;
            Logger::get_instance().log(BasicLogger | ConsoleLogger, "GroupService::add_group success {}", id);
            return std::make_pair(id.data(), "");
        }
        catch (const std::exception &e) {
            Logger::get_instance().log(BasicLogger | ConsoleLogger, "Exception in GroupService::add_group: %s",
                                       e.what());
            return std::make_pair(std::nullopt, e.what());
        }
    }

    auto GroupService::get_group(const bsoncxx::oid &group_id) -> schema::result<schema::DbGroup, std::string> {
        Logger::get_instance().log(BasicLogger | ConsoleLogger, "Enter GroupService::get_group");
        try {
            const auto result = group_collection.find_one(make_document(kvp(schema::key::bson_id, group_id)));
            if (result.has_value()) {
                const auto &db_group = result.value();
                const auto group = schema::DbGroup::from_bson(db_group);
                return std::make_pair(group, "");
            }
            return std::make_pair(std::nullopt, "Group not found");
        }
        catch (const std::exception &e) {
            Logger::get_instance().log(BasicLogger | ConsoleLogger, "Exception in GroupService::get_group: %s",
                                       e.what());
            return std::make_pair(std::nullopt, e.what());
        }
    }

    auto GroupService::group_exist(const bsoncxx::oid &group_id) -> schema::result<bool, std::string> {
        Logger::get_instance().log(BasicLogger | ConsoleLogger, "Enter GroupService::group_exist");
        const auto bson_group_id = bsoncxx::oid{group_id};
        try {
            if (const auto result = group_collection.find_one(make_document(kvp(schema::key::bson_id, bson_group_id)));
                result.has_value()) {
                Logger::get_instance().log(BasicLogger | ConsoleLogger, "GroupService::group_exist: group_id: {} exist",
                                           group_id.to_string());
                return std::make_pair(true, "");
            }

            Logger::get_instance().log(BasicLogger | ConsoleLogger, "GroupService::group_exist: group_id: {} not exist",
                                       group_id.to_string());
            return std::make_pair(false, "");
        }
        catch (const std::exception &e) {
            Logger::get_instance().log(BasicLogger | ConsoleLogger, "Exception in GroupService::group_exist: %s",
                                       e.what());
            return std::make_pair(false, e.what());
        }
    }

    auto GroupService::add_member(const bsoncxx::oid &group_id, const bsoncxx::oid &member_id)
            -> schema::result<bool, std::string> {
        Logger::get_instance().log(BasicLogger | ConsoleLogger, "Enter GroupService::add_member");
        try {
            const auto filter = make_document(kvp(schema::key::bson_id, group_id));
            const auto update = make_document("$addToSet", make_document(kvp(schema::key::members_id, member_id)));

            if (const auto result = group_collection.update_one(filter.view(), update.view()); !result.has_value()) {
                Logger::get_instance().log(BasicLogger | ConsoleLogger, spdlog::level::err,
                                           "GroupService::add_member failed");
                return std::make_pair(false, "GroupService::add_member failed");
            }
            return std::make_pair(true, "");
        }
        catch (const std::exception &e) {
            Logger::get_instance().log(BasicLogger | ConsoleLogger, spdlog::level::err,
                                       "Exception in GroupService::add_member: {}", e.what());
            return std::make_pair(false, e.what());
        }
    }

    auto GroupService::remove_member(const bsoncxx::oid &group_id, const bsoncxx::oid &member_id)
            -> schema::result<bool, std::string> {
        Logger::get_instance().log(BasicLogger | ConsoleLogger, "Enter GroupService::remove_member");
        try {
            const auto filter = make_document(kvp(schema::key::bson_id, group_id));
            const auto update = make_document("$pull", make_document(kvp(schema::key::members_id, member_id)));

            if (const auto result = group_collection.update_one(filter.view(), update.view()); !result.has_value()) {
                Logger::get_instance().log(BasicLogger | ConsoleLogger, spdlog::level::err,
                                           "GroupService::remove_member failed");
                return std::make_pair(false, "GroupService::remove_member failed");
            }
            return std::make_pair(true, "");
        }
        catch (const std::exception &e) {
            Logger::get_instance().log(BasicLogger | ConsoleLogger, spdlog::level::err,
                                       "Exception in GroupService::remove_member: {}", e.what());
            return std::make_pair(false, e.what());
        }
    }

    auto GroupService::get_members(const bsoncxx::oid &group_id)
            -> schema::result<std::vector<bsoncxx::oid>, std::string> {}

    auto GroupService::rename(const bsoncxx::oid &group_id, const std::string &new_name)
            -> schema::result<bool, std::string> {}

    auto GroupService::is_member(const bsoncxx::oid &group_id, const bsoncxx::oid &member_id)
            -> schema::result<bool, std::string> {}

    auto GroupService::get_all_groups() -> schema::result<std::vector<schema::DbGroup>, std::string> {}

    auto GroupService::delete_group(const bsoncxx::oid &group_id) -> schema::result<bool, std::string> {}

    auto GroupService::add_bucket(const bsoncxx::oid &group_id, const bsoncxx::oid &bucket_id)
            -> schema::result<bool, std::string> {}

    auto GroupService::remove_bucket(const bsoncxx::oid &group_id, const bsoncxx::oid &bucket_id)
            -> schema::result<bool, std::string> {}

    auto GroupService::get_bucket_ids(const bsoncxx::oid &group_id)
            -> schema::result<std::vector<bsoncxx::oid>, std::string> {}

    auto GroupService::is_bucket_exist(const bsoncxx::oid &group_id, const bsoncxx::oid &bucket_id)
            -> schema::result<bool, std::string> {}
} // namespace service_delight
