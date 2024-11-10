//
// Created by cvrain on 24-11-10.
//

#include "storage_service.hpp"

#include "service/log_service.hpp"

using namespace service_delight;

auto StorageService::init() {
    Logger::get_instance().log(ConsoleLogger, "StorageService init");
    data_source_collection = MongoProvider::get_instance().get_collection(schema::key::collection::data_source.data());
}

auto StorageService::append_storage(schema::DbDataSource *data_source) -> schema::result<bool, std::string_view> {
    Logger::get_instance().log(ConsoleLogger, "StorageService append_storage");

    // 检查数据库中是否存在同一个名称
    try {
        const auto storage_name = data_source->name;
        if (const auto [result, error_msg] = check_storage_exist(data_source->id);
            result.has_value() && result.value() == true) {
            Logger::get_instance().log(ConsoleLogger, spdlog::level::warn,
                                       "StorageService append_storage: storage name: {} has been exist", storage_name);
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
        const auto result = data_source_collection.find(make_document());
        std::vector<bsoncxx::oid> ids;
        for (const auto &item: result) {
            ids.emplace_back(item[schema::key::bson_id].get_oid().value);
        }
        return std::make_pair(ids, "");
    }
    catch (const std::exception &e) {
        service_delight::Logger::get_instance().log(ConsoleLogger | BasicLogger, spdlog::level::err,
                                                    "StorageService list_all_storage_ids: {}", e.what());
        return std::make_pair(std::nullopt, e.what());
    }
}
auto StorageService::list_all_storages() -> schema::result<std::vector<bsoncxx::document::value>, std::string_view>{

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
