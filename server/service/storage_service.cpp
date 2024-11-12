//
// Created by cvrain on 24-11-10.
//

#include "storage_service.hpp"

#include "service/log_service.hpp"
#include "utils/item.h"

using namespace service_delight;

auto StorageService::init() -> void {
    Logger::get_instance().log(ConsoleLogger, "StorageService init");
    data_source_collection = MongoProvider::get_instance().get_collection(schema::key::collection::data_source.data());
}

auto StorageService::append_storage(schema::DbDataSource *data_source) -> schema::result<bool, std::string_view> {
    Logger::get_instance().log(ConsoleLogger, "StorageService append_storage");

    // 检查是否能够连接到MinIO服务器
    if (const auto minio_connection = util_delight::Item::check_minio_server_connection(data_source->url);
        !minio_connection) {
        return std::make_pair(false, "minio server connection failed");
    }

    // 检查数据库中是否存在同一个名称
    try {
        const auto storage_name = data_source->name;
        if (const auto [result, error_msg] = check_storage_exist(data_source->id);
            result.has_value() && result.value() == true) {
            Logger::get_instance().log(ConsoleLogger, spdlog::level::warn,
                                       "StorageService::append_storage: storage name: {} has been exist", storage_name);
            return std::make_pair(false, "storage name has been exist");
        }
    }
    catch (const std::exception &e) {
        Logger::get_instance().log(ConsoleLogger | BasicLogger, spdlog::level::err, "StorageService append_storage: {}",
                                   e.what());
        return std::make_pair(false, e.what());
    }

    // 尝试插入数据
    try {
        const auto insert_content = data_source->get_document();
        if (const auto result = data_source_collection.insert_one(insert_content.view());
            result.has_value() && result.value().result().inserted_count() == 1) {
            Logger::get_instance().log(ConsoleLogger, "StorageService append_storage: insert storage success");
            return std::make_pair(true, "");
        }
        return std::make_pair(false, "insert storage failed");
    }
    catch (const std::exception &e) {
        service_delight::Logger::get_instance().log(ConsoleLogger | BasicLogger, spdlog::level::err,
                                                    "StorageService append_storage: {}", e.what());
        return std::make_pair(false, e.what());
    }
}
auto StorageService::get_storage_by_id(const bsoncxx::oid &id)
        -> schema::result<bsoncxx::document::value, std::string_view> {
    service_delight::Logger::get_instance().log(ConsoleLogger, "StorageService get_storage_by_id");
    try {
        const auto result = data_source_collection.find_one(make_document(kvp(schema::key::bson_id, id)));
        if (!result.has_value()) {
            return std::make_pair(std::nullopt, "storage not found");
        }
        return std::make_pair(result.value(), "");
    }
    catch (const std::exception &e) {
        service_delight::Logger::get_instance().log(ConsoleLogger | BasicLogger, spdlog::level::err,
                                                    "StorageService get_storage_by_id: {}", e.what());
        return std::make_pair(std::nullopt, e.what());
    }
}
auto StorageService::get_storage_by_name(const std::string &name)
        -> schema::result<bsoncxx::document::value, std::string_view> {
    service_delight::Logger::get_instance().log(ConsoleLogger, "StorageService get_storage_by_name");
    try {
        const auto result = data_source_collection.find_one(make_document(kvp(schema::key::name, name)));
        if (!result.has_value()) {
            return std::make_pair(std::nullopt, "storage not found");
        }
        return std::make_pair(result.value(), "");
    }
    catch (const std::exception &e) {
        service_delight::Logger::get_instance().log(ConsoleLogger | BasicLogger, spdlog::level::err,
                                                    "StorageService get_storage_by_name: {}", e.what());
        return std::make_pair(std::nullopt, e.what());
    }
}
auto StorageService::list_all_storage_ids() -> schema::result<std::vector<bsoncxx::oid>, std::string_view> {
    service_delight::Logger::get_instance().log(ConsoleLogger, "StorageService list_all_storage_ids");
    try {
        auto result = data_source_collection.find(make_document());
        std::vector<bsoncxx::oid> ids;
        for (auto &&it: result) {
            const auto one_id = it.find(schema::key::bson_id)->get_oid().value;
            ids.emplace_back(one_id);
        }
        return std::make_pair(ids, "");
    }
    catch (const std::exception &e) {
        service_delight::Logger::get_instance().log(ConsoleLogger | BasicLogger, spdlog::level::err,
                                                    "StorageService list_all_storage_ids: {}", e.what());
        return std::make_pair(std::nullopt, e.what());
    }
}
auto StorageService::list_all_storages() -> schema::result<std::vector<bsoncxx::document::value>, std::string_view> {
    service_delight::Logger::get_instance().log(ConsoleLogger, "StorageService list_all_storages");
    try {
        auto result = data_source_collection.find(make_document());
        std::vector<bsoncxx::document::value> storages;
        for (auto &&item: result) {
            storages.emplace_back(item);
        }
        return std::make_pair(storages, "");
    }
    catch (const std::exception &e) {
        service_delight::Logger::get_instance().log(ConsoleLogger | BasicLogger, spdlog::level::err,
                                                    "StorageService list_all_storages: {}", e.what());
        return std::make_pair(std::nullopt, e.what());
    }
}
auto StorageService::remove_storage(const bsoncxx::oid &id) -> schema::result<bool, std::string_view> {
    service_delight::Logger::get_instance().log(ConsoleLogger, "StorageService remove_storage");

    try {
        if (const auto result = data_source_collection.delete_one(make_document(kvp(schema::key::bson_id, id)));
            result.has_value() && result.value().result().deleted_count() == 1) {
            return std::make_pair(true, "");
        }
        return std::make_pair(false, "delete storage failed");
    }
    catch (const std::exception &e) {
        service_delight::Logger::get_instance().log(ConsoleLogger | BasicLogger, spdlog::level::err,
                                                    "StorageService remove_storage: {}", e.what());
        return std::make_pair(false, e.what());
    }
}
auto StorageService::update_storage(schema::DbDataSource *data_source) -> schema::result<bool, std::string_view> {
    service_delight::Logger::get_instance().log(ConsoleLogger, "StorageService update_storage");
    try {
        const auto update_id = data_source->id;
        const auto update_document = data_source->get_document().view();
        const auto result =
                data_source_collection.update_one(make_document(kvp(schema::key::bson_id, update_id)), update_document);
        if (result.has_value() && result.value().result().modified_count() == 1) {
            return std::make_pair(true, "");
        }
        return std::make_pair(false, "update storage failed");
    }
    catch (const std::exception &e) {
        service_delight::Logger::get_instance().log(ConsoleLogger | BasicLogger, spdlog::level::err,
                                                    "StorageService update_storage: {}", e.what());
        return std::make_pair(false, e.what());
    }
}

auto StorageService::check_storage_exist(const bsoncxx::oid &id) -> schema::result<bool, std::string_view> {
    service_delight::Logger::get_instance().log(ConsoleLogger, "StorageService check_storage_exist");

    try {
        const auto result =
                data_source_collection.find_one(bsoncxx::builder::basic::make_document(kvp(schema::key::bson_id, id)));
        return std::make_pair(result.has_value(), "");
    }
    catch (const std::exception &e) {
        service_delight::Logger::get_instance().log(ConsoleLogger | BasicLogger, spdlog::level::err,
                                                    "StorageService check_storage_exist: {}", e.what());
        return std::make_pair(false, e.what());
    }
}

auto StorageService::check_storage_exist(const std::string &name) -> schema::result<bool, std::string_view> {
    service_delight::Logger::get_instance().log(ConsoleLogger, "StorageService check_storage_exist");
    try {
        const auto result =
                data_source_collection.find_one(bsoncxx::builder::basic::make_document(kvp(schema::key::name, name)));
        return std::make_pair(result.has_value(), "");
    }
    catch (const std::exception &e) {
        service_delight::Logger::get_instance().log(ConsoleLogger | BasicLogger, spdlog::level::err,
                                                    "StorageService check_storage_exist:{}", e.what());
        return std::make_pair(false, e.what());
    }
}
