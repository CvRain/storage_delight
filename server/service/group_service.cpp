//
// Created by cvrain on 24-10-5.
//

#include "group_service.hpp"
#include "basic_value.hpp"
#include "logger.hpp"
#include "schema_key.hpp"

using namespace model_delight::bson_operator;

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
            const auto update = make_document(
                    kvp(update::array::add_to_set, make_document(kvp(schema::key::members_id, member_id))));

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
    auto GroupService::add_members(const bsoncxx::oid &group_id, const std::vector<bsoncxx::oid> &member_ids)
            -> schema::result<bool, std::string> {
        Logger::get_instance().log(BasicLogger | ConsoleLogger, "Enter GroupService::add_members");
        try {
            const auto filter = make_document(kvp(schema::key::bson_id, group_id));

            //todo
            bsoncxx::builder::basic::array members;
            for(const auto &member_id : member_ids) {
                members.append(member_id);
            }

            const auto update = make_document(kvp(update::array::push,
                make_document(kvp(schema::key::members_id, members))));


            if(const auto result = group_collection.update_one(filter.view(), update.view()); !result.has_value()) {
                Logger::get_instance().log(BasicLogger | ConsoleLogger, spdlog::level::err,
                                           "GroupService::add_members failed");
                return std::make_pair(false, "GroupService::add_members failed");
            }
            return std::make_pair(true, "");

        }catch (const std::exception &e) {
            Logger::get_instance().log(BasicLogger | ConsoleLogger, spdlog::level::err,
                                       "Exception in GroupService::add_members: {}", e.what());
            return std::make_pair(false, e.what());
        }
    }

    auto GroupService::remove_member(const bsoncxx::oid &group_id, const bsoncxx::oid &member_id)
            -> schema::result<bool, std::string> {
        Logger::get_instance().log(BasicLogger | ConsoleLogger, "Enter GroupService::remove_member");
        try {
            const auto filter = make_document(kvp(schema::key::bson_id, group_id));
            const auto update =
                    make_document(kvp(update::array::pull, make_document(kvp(schema::key::members_id, member_id))));

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
            -> schema::result<std::vector<bsoncxx::oid>, std::string> {
        Logger::get_instance().log(BasicLogger | ConsoleLogger, "Enter GroupService::get_members");
        try {
            const auto filter = make_document(kvp(schema::key::bson_id, group_id));
            if (const auto doc = group_collection.find_one(filter.view()); doc.has_value()) {
                const auto [value] = doc.value().find(schema::key::members_id)->get_array();
                std::vector<bsoncxx::oid> member_ids;
                for (const auto &member: value) {
                    member_ids.emplace_back(member.get_oid().value);
                }
                return std::make_pair(member_ids, "");
            }
            return std::make_pair(std::nullopt, "Group not found");
        }
        catch (const std::exception &e) {
            Logger::get_instance().log(BasicLogger | ConsoleLogger, spdlog::level::err,
                                       "Exception in GroupService::get_members: {}", e.what());
            return std::make_pair(std::nullopt, e.what());
        }
    }

    auto GroupService::rename(const bsoncxx::oid &group_id, const std::string &new_name)
            -> schema::result<bool, std::string> {
        Logger::get_instance().log(BasicLogger | ConsoleLogger, "Enter GroupService::rename");
        const auto filter = make_document(kvp(schema::key::bson_id, group_id));
        try {
            const auto update = make_document(kvp(update::field::set, make_document(kvp(schema::key::name, new_name))));
            if (const auto result = group_collection.update_one(filter.view(), update.view()); !result.has_value()) {
                Logger::get_instance().log(BasicLogger | ConsoleLogger, spdlog::level::err,
                                           "GroupService::rename failed");
                return std::make_pair(false, "GroupService::rename failed");
            }
            return std::make_pair(true, "");
        }
        catch (const std::exception &e) {
            Logger::get_instance().log(BasicLogger | ConsoleLogger, spdlog::level::err,
                                       "Exception in GroupService::rename: {}", e.what());
            return std::make_pair(false, e.what());
        }
    }

    auto GroupService::is_member(const bsoncxx::oid &group_id, const bsoncxx::oid &member_id)
            -> schema::result<bool, std::string> {
        Logger::get_instance().log(BasicLogger | ConsoleLogger, "Enter GroupService::is_member");
        try {
            const auto filter = make_document(kvp(schema::key::bson_id, group_id));
            if (const auto doc = group_collection.find_one(filter.view()); doc.has_value()) {
                for (const auto value = doc.value().find(schema::key::members_id)->get_array().value;
                     const auto &member: value) {
                    if (member.get_oid().value == member_id) {
                        return std::make_pair(true, "");
                    }
                }
            }
            return std::make_pair(false, "");
        }
        catch (const std::exception &e) {
            Logger::get_instance().log(BasicLogger | ConsoleLogger, spdlog::level::err,
                                       "Exception in GroupService::is_member: {}", e.what());
            return std::make_pair(false, e.what());
        }
    }

    auto GroupService::get_all_groups() -> schema::result<std::vector<bsoncxx::document::value>, std::string> {
        Logger::get_instance().log(BasicLogger | ConsoleLogger, "Enter GroupService::get_all_groups");
        try {
            std::vector<bsoncxx::document::value> groups;
            auto cursor = group_collection.find({});
            for (auto doc: cursor) {
                groups.emplace_back(doc);
            }
            return std::make_pair(groups, "");
        }
        catch (const std::exception &e) {
            Logger::get_instance().log(BasicLogger | ConsoleLogger, spdlog::level::err,
                                       "Exception in GroupService::get_all_groups: {}", e.what());
            return std::make_pair(std::nullopt, e.what());
        }
    }

    auto GroupService::delete_group(const bsoncxx::oid &group_id) -> schema::result<bool, std::string> {
        Logger::get_instance().log(BasicLogger | ConsoleLogger, "Enter GroupService::delete_group");
        try {
            const auto filter = make_document(kvp(schema::key::bson_id, group_id));
            if (const auto result = group_collection.delete_one(filter.view()); result && result->deleted_count() > 0) {
                return std::make_pair(true, "");
            }
            Logger::get_instance().log(BasicLogger | ConsoleLogger, spdlog::level::err,
                                       "GroupService::delete_group failed");
            return std::make_pair(false, "GroupService::delete_group failed");
        }
        catch (const std::exception &e) {
            Logger::get_instance().log(BasicLogger | ConsoleLogger, spdlog::level::err,
                                       "Exception in GroupService::delete_group: {}", e.what());
            return std::make_pair(false, e.what());
        }
    }

    auto GroupService::add_bucket(const bsoncxx::oid &group_id, const bsoncxx::oid &bucket_id)
            -> schema::result<bool, std::string> {
        Logger::get_instance().log(BasicLogger | ConsoleLogger, "Enter GroupService::add_bucket");
        try {
            const auto filter = make_document(kvp(schema::key::bson_id, group_id));
            const auto update =
                    make_document(kvp(update::array::push, make_document(kvp(schema::key::bucket_id, bucket_id))));
            if (const auto result = group_collection.update_one(filter.view(), update.view()); !result.has_value()) {
                Logger::get_instance().log(BasicLogger | ConsoleLogger, spdlog::level::err,
                                           "GroupService::add_bucket failed");
            }
            return std::make_pair(true, "");
        }
        catch (const std::exception &e) {
            Logger::get_instance().log(BasicLogger | ConsoleLogger, spdlog::level::err,
                                       "Exception in GroupService::add_bucket: {}", e.what());
            return std::make_pair(false, e.what());
        }
    }

    auto GroupService::remove_bucket(const bsoncxx::oid &group_id, const bsoncxx::oid &bucket_id)
            -> schema::result<bool, std::string> {
        Logger::get_instance().log(BasicLogger | ConsoleLogger, "Enter GroupService::remove_bucket");
        try {
            const auto filter = make_document(kvp(schema::key::bson_id, group_id));
            const auto update =
                    make_document(kvp(update::array::pull, make_document(kvp(schema::key::bucket_id, bucket_id))));
            if (const auto result = group_collection.update_one(filter.view(), update.view()); !result.has_value()) {
                Logger::get_instance().log(BasicLogger | ConsoleLogger, spdlog::level::err,
                                           "GroupService::remove_bucket failed");
                return std::make_pair(false, "GroupService::remove_bucket failed");
            }
            return std::make_pair(true, "");
        }
        catch (const std::exception &e) {
            Logger::get_instance().log(BasicLogger | ConsoleLogger, spdlog::level::err,
                                       "Exception in GroupService::remove_bucket: {}", e.what());
            return std::make_pair(false, e.what());
        }
    }

    auto GroupService::get_bucket_ids(const bsoncxx::oid &group_id)
            -> schema::result<std::vector<bsoncxx::oid>, std::string> {
        Logger::get_instance().log(BasicLogger | ConsoleLogger, "Enter GroupService::get_bucket_ids");
        try {
            const auto filter = make_document(kvp(schema::key::bson_id, group_id));
            if (const auto doc = group_collection.find_one(filter.view()); doc.has_value()) {
                std::vector<bsoncxx::oid> bucket_ids;
                for (const auto value = doc.value().find(schema::key::bucket_id)->get_array().value;
                     const auto &bucket: value) {
                    bucket_ids.emplace_back(bucket.get_oid().value);
                }
                return std::make_pair(bucket_ids, "");
            }
            return std::make_pair(std::nullopt, "GroupService::get_bucket_ids failed");
        }
        catch (const std::exception &e) {
            Logger::get_instance().log(BasicLogger | ConsoleLogger, spdlog::level::err,
                                       "Exception in GroupService::get_bucket_ids: {}", e.what());
            return std::make_pair(std::nullopt, e.what());
        }
    }

    auto GroupService::is_bucket_exist(const bsoncxx::oid &group_id, const bsoncxx::oid &bucket_id)
            -> schema::result<bool, std::string> {
        Logger::get_instance().log(BasicLogger | ConsoleLogger, "Enter GroupService::is_bucket_exist");
        try {
            const auto filter = make_document(kvp(schema::key::bson_id, group_id));
            if (const auto doc = group_collection.find_one(filter.view()); doc.has_value()) {
                for (const auto value = doc.value().find(schema::key::bucket_id)->get_array().value;
                     const auto &bucket: value) {
                    if (bucket.get_oid().value == bucket_id) {
                        return std::make_pair(true, "");
                    }
                }
            }
            return std::make_pair(false, "GroupService::is_bucket_exist failed");
        }
        catch (const std::exception &e) {
            Logger::get_instance().log(BasicLogger | ConsoleLogger, spdlog::level::err,
                                       "Exception in GroupService::is_bucket_exist: {}", e.what());
            return std::make_pair(false, e.what());
        }
    }
} // namespace service_delight
