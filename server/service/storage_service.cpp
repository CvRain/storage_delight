//
// Created by cvrain on 24-11-10.
//

#include "storage_service.hpp"

#include "client.hpp"
#include "service/log_service.hpp"
#include "utils/item.h"

using namespace service_delight;

/**
 * @brief 初始化存储服务
 */
auto StorageService::init() -> void {
    Logger::get_instance().log(ConsoleLogger, "StorageService init");
    data_source_collection = MongoProvider::get_instance().get_collection(schema::key::collection::data_source.data());
    is_init                = true;
}

auto StorageService::init_flag() const -> const bool & { return is_init; }

/**
 * @brief 添加存储源
 * @param data_source 接受DbDataSource结构
 */
auto StorageService::append_storage(schema::DbDataSource *data_source)
        -> schema::result<bsoncxx::oid, std::string_view> {
    Logger::get_instance().log(ConsoleLogger, "StorageService append_storage");

    // 检查是否能够连接到MinIO服务器
    try {
        if (const auto minio_connection = util_delight::Item::check_minio_server_connection(data_source->url);
            !minio_connection)
        {
            return std::make_pair(std::nullopt, "minio server connection failed");
        }
    }
    catch (const std::exception &e) {
        Logger::get_instance().log(
                ConsoleLogger | BasicLogger, spdlog::level::err, "StorageService append_storage: {}", e.what());
        return std::make_pair(std::nullopt, e.what());
    }

    // 检查数据库中是否存在同一个名称
    try {
        const auto storage_name = data_source->name;
        if (const auto [result, error_msg] = is_exist(storage_name); result.has_value() && result.value() == true) {
            Logger::get_instance().log(ConsoleLogger,
                                       spdlog::level::info,
                                       "StorageService::append_storage: storage name: {} has been exist",
                                       storage_name);
            throw std::runtime_error("storage name has been exist");
        }
    }
    catch (const std::exception &e) {
        Logger::get_instance().log(
                ConsoleLogger | BasicLogger, spdlog::level::err, "StorageService append_storage: {}", e.what());
        return std::make_pair(std::nullopt, e.what());
    }

    // 尝试插入数据
    try {
        const auto insert_content = data_source->get_document();
        if (const auto result = data_source_collection.insert_one(insert_content.view());
            result.has_value() && result.value().result().inserted_count() == 1)
        {
            Logger::get_instance().log(ConsoleLogger, "StorageService append_storage: insert storage success");
            return std::make_pair(result.value().inserted_id().get_oid().value, "");
        }
        throw std::runtime_error("insert storage failed");
    }
    catch (const std::exception &e) {
        service_delight::Logger::get_instance().log(
                ConsoleLogger | BasicLogger, spdlog::level::err, "StorageService append_storage: {}", e.what());
        return std::make_pair(std::nullopt, e.what());
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
        service_delight::Logger::get_instance().log(
                ConsoleLogger | BasicLogger, spdlog::level::err, "StorageService get_storage_by_id: {}", e.what());
        return std::make_pair(std::nullopt, e.what());
    }
}

auto StorageService::get_storage_by_name(const std::string_view &name)
        -> schema::result<bsoncxx::document::value, std::string_view> {
    service_delight::Logger::get_instance().log(ConsoleLogger, "StorageService get_storage_by_name");
    try {
        const auto result = data_source_collection.find_one(make_document(kvp(schema::key::name, name.data())));
        if (!result.has_value()) {
            return std::make_pair(std::nullopt, "storage not found");
        }
        return std::make_pair(result.value(), "");
    }
    catch (const std::exception &e) {
        service_delight::Logger::get_instance().log(
                ConsoleLogger | BasicLogger, spdlog::level::err, "StorageService get_storage_by_name: {}", e.what());
        return std::make_pair(std::nullopt, e.what());
    }
}

auto StorageService::list_all_storage_ids() -> schema::result<std::vector<bsoncxx::oid>, std::string_view> {
    service_delight::Logger::get_instance().log(ConsoleLogger, "Enter StorageService::list_all_storage_ids");
    try {
        auto                      result = data_source_collection.find(make_document());
        std::vector<bsoncxx::oid> ids;
        for (auto &&it: result) {
            const auto one_id = it.find(schema::key::bson_id)->get_oid().value;
            ids.emplace_back(one_id);
        }
        return std::make_pair(ids, "");
    }
    catch (const std::exception &e) {
        service_delight::Logger::get_instance().log(
                ConsoleLogger | BasicLogger, spdlog::level::err, "StorageService list_all_storage_ids: {}", e.what());
        return std::make_pair(std::nullopt, e.what());
    }
}

auto StorageService::list_all_storages() -> schema::result<std::vector<bsoncxx::document::value>, std::string_view> {
    service_delight::Logger::get_instance().log(ConsoleLogger, "StorageService::list_all_storages");
    try {
        auto result = data_source_collection.find(make_document());

        std::vector<bsoncxx::document::value> storages;
        for (auto &&item: result) {
            service_delight::Logger::get_instance().log(ConsoleLogger,
                                                        spdlog::level::debug,
                                                        "StorageService list_all_storages: Find source:{}",
                                                        item.find(schema::key::bson_id)->get_oid().value.to_string());
            storages.emplace_back(item);
        }
        return std::make_pair(storages, "");
    }
    catch (const std::exception &e) {
        service_delight::Logger::get_instance().log(
                ConsoleLogger | BasicLogger, spdlog::level::err, "StorageService list_all_storages: {}", e.what());
        return std::make_pair(std::nullopt, e.what());
    }
}
auto StorageService::remove_storage(const bsoncxx::oid &id) -> schema::result<bool, std::string_view> {
    service_delight::Logger::get_instance().log(ConsoleLogger, "StorageService remove_storage");

    try {
        if (const auto result = data_source_collection.delete_one(make_document(kvp(schema::key::bson_id, id)));
            result && result.value().result().deleted_count() > 0)
        {
            return std::make_pair(true, "");
        }
        return std::make_pair(false, "Remove storage source failed");
    }
    catch (const std::exception &e) {
        service_delight::Logger::get_instance().log(
                ConsoleLogger | BasicLogger, spdlog::level::err, "StorageService remove_storage: {}", e.what());
        return std::make_pair(false, e.what());
    }
}
auto StorageService::update_storage(schema::DbDataSource *data_source) -> schema::result<bool, std::string_view> {
    service_delight::Logger::get_instance().log(ConsoleLogger, "StorageService update_storage");
    try {
        const auto update_id       = data_source->id;
        const auto update_document = data_source->get_document().view();
        const auto result =
                data_source_collection.update_one(make_document(kvp(schema::key::bson_id, update_id)), update_document);
        if (result.has_value() && result.value().result().modified_count() == 1) {
            return std::make_pair(true, "");
        }
        return std::make_pair(false, "update storage failed");
    }
    catch (const std::exception &e) {
        service_delight::Logger::get_instance().log(
                ConsoleLogger | BasicLogger, spdlog::level::err, "StorageService update_storage: {}", e.what());
        return std::make_pair(false, e.what());
    }
}

auto StorageService::is_exist(const bsoncxx::oid &id) -> schema::result<bool, std::string_view> {
    service_delight::Logger::get_instance().log(ConsoleLogger, "StorageService check_storage_exist");

    try {
        const auto result =
                data_source_collection.find_one(bsoncxx::builder::basic::make_document(kvp(schema::key::bson_id, id)));
        return std::make_pair(result.has_value(), "");
    }
    catch (const std::exception &e) {
        service_delight::Logger::get_instance().log(
                ConsoleLogger | BasicLogger, spdlog::level::err, "StorageService check_storage_exist: {}", e.what());
        return std::make_pair(false, e.what());
    }
}

auto StorageService::is_exist(const std::string &name) -> schema::result<bool, std::string_view> {
    service_delight::Logger::get_instance().log(ConsoleLogger, "StorageService check_storage_exist");
    try {
        const auto result =
                data_source_collection.find_one(bsoncxx::builder::basic::make_document(kvp(schema::key::name, name)));
        return std::make_pair(result.has_value(), "");
    }
    catch (const std::exception &e) {
        service_delight::Logger::get_instance().log(
                ConsoleLogger | BasicLogger, spdlog::level::err, "StorageService check_storage_exist:{}", e.what());
        return std::make_pair(false, e.what());
    }
}

auto StorageService::active_storage(const bsoncxx::oid &id) -> schema::result<bool, std::string_view> {
    Logger::get_instance().log(ConsoleLogger, "Enter StorageService active_storage");

    try {
        // 获得存储源
        const auto [document, error] = get_storage_by_id(id);
        if (not document.has_value()) {
            throw std::runtime_error(error.data());
        }

        const auto storage  = schema::DbDataSource::from_bson(document.value());
        auto       provider = storage_delight::core::Client::make_provider(storage.access_key, storage.secret_key);
        // auto provider = std::make_shared<minio::creds::StaticProvider>(storage.access_key, storage.secret_key);
        auto url = minio::s3::BaseUrl{storage.url, storage.is_https};

        Logger::get_instance().log(
                ConsoleLogger | BasicLogger, spdlog::level::info, "Active storage: {}", storage.name);
        client_group.push_back(storage.id, std::make_shared<storage_delight::core::Client>(url, std::move(provider)));
    }
    catch (const std::exception &e) {
        Logger::get_instance().log(
                ConsoleLogger | BasicLogger, spdlog::level::err, "StorageService active_storage:{}", e.what());
        return std::make_pair(false, e.what());
    }
    return std::make_pair(true, "");
}

auto StorageService::active_all_storage() -> schema::result<bool, std::string_view> {
    Logger::get_instance().log(ConsoleLogger, "Enter StorageService::active_storage");

    try {
        const auto [documents, error] = list_all_storages();
        if (not documents.has_value()) {
            throw std::runtime_error(error.data());
        }
        Logger::get_instance().log(
                ConsoleLogger, "StorageService::active_storage find {} storages", documents.value().size());


        for (const auto &document: documents.value()) {
            const auto storage  = schema::DbDataSource::from_bson(document);
            auto       provider = storage_delight::core::Client::make_provider(storage.access_key, storage.secret_key);
            auto       url      = minio::s3::BaseUrl{storage.url, storage.is_https};

            Logger::get_instance().log(
                    ConsoleLogger | BasicLogger, spdlog::level::info, "Active storage: {}", storage.name);
            client_group.push_back(storage.id,
                                   std::make_shared<storage_delight::core::Client>(url, std::move(provider)));
        }
    }
    catch (std::exception &e) {
        Logger::get_instance().log(
                ConsoleLogger | BasicLogger, spdlog::level::err, "StorageService active_storage: {}", e.what());
        return std::make_pair(false, e.what());
    }
    return std::make_pair(true, "");
}

auto StorageService::is_active(const bsoncxx::oid &id) -> schema::result<bool, std::string_view> {
    Logger::get_instance().log(ConsoleLogger, "Enter StorageService is_active");
    return std::make_pair(client_group.is_exist(id), "");
}

auto StorageService::inactive_storage(const bsoncxx::oid &id) -> schema::result<bool, std::string_view> {
    Logger::get_instance().log(ConsoleLogger, "Enter StorageService inactive_storage");

    try {
        client_group.remove(id);
    }
    catch (const std::exception &e) {
        Logger::get_instance().log(
                ConsoleLogger | BasicLogger, spdlog::level::err, "StorageService::inactive_storage {}", e.what());
        return std::make_pair(false, e.what());
    }

    return std::make_pair(true, "");
}

auto StorageService::inactive_all_storage() -> schema::result<bool, std::string_view> {
    Logger::get_instance().log(ConsoleLogger, "Enter StorageService inactive_all_storage");

    try {
        client_group.clear();
    }
    catch (const std::exception &e) {
        Logger::get_instance().log(
                ConsoleLogger | BasicLogger, spdlog::level::err, "StorageService::inactive_all_storage {}", e.what());
        return std::make_pair(std::nullopt, e.what());
    }
    return std::make_pair(true, "");
}

auto StorageService::source_name(const bsoncxx::oid &source_id) -> schema::result<std::string, std::string_view> {
    Logger::get_instance().log(ConsoleLogger, "Enter StorageService source_name");

    try {
        const auto document = data_source_collection.find_one(make_document(kvp(schema::key::bson_id, source_id)));
        if (not document.has_value()) {
            return std::make_pair(std::nullopt, "No such source");
        }
        const auto source_name = document->find(schema::key::name)->get_string().value;
        return std::make_pair(source_name.data(), "");
    }
    catch (const std::exception &e) {
        Logger::get_instance().log(
                ConsoleLogger | BasicLogger, spdlog::level::err, "StorageService::source_name {}", e.what());
        return std::make_pair(std::nullopt, e.what());
    }
}

auto StorageService::get_client(const bsoncxx::oid &source_id)
        -> std::optional<std::shared_ptr<storage_delight::core::Client>> {
    std::lock_guard<std::mutex> lock(mutex_);

    Logger::get_instance().log(ConsoleLogger, "Enter StorageService::get_client");
    try {
        if (client_group.is_exist(source_id)) {
            Logger::get_instance().log(
                    ConsoleLogger, "StorageService::get_client {} client exist", source_id.to_string());
            if (const auto client = client_group.get_client(source_id); client == nullptr) {
                Logger::get_instance().log(ConsoleLogger | BasicLogger,
                                           spdlog::level::err,
                                           "StorageService::get_client {} client null",
                                           source_id.to_string());
                return std::nullopt;
            }
            return client_group.get_client(source_id);
        }
        return std::nullopt;
    }
    catch (const std::exception &) {
        throw;
    }
}

auto StorageService::generate_client(const bsoncxx::oid &source_id) -> std::shared_ptr<storage_delight::core::Client> {
    Logger::get_instance().log(ConsoleLogger, "Enter StorageService::generate_client");
    try {
        const auto source_document =
                data_source_collection.find_one(make_document(kvp(schema::key::bson_id, source_id)));
        if (not source_document.has_value()) {
            return nullptr;
        }
        const auto source   = schema::DbDataSource::from_bson(source_document.value());
        const auto url      = minio::s3::BaseUrl{source.url, source.is_https};
        auto       provider = storage_delight::core::Client::make_provider(source.access_key, source.secret_key);
        return std::make_shared<storage_delight::core::Client>(url, std::move(provider));
    }
    catch (const std::exception &e) {
        Logger::get_instance().log(
                ConsoleLogger | BasicLogger, spdlog::level::err, "StorageService::generate_client {}", e.what());
        throw;
    }
}
