//
// Created by cvrain on 24-11-13.
//

#include "bucket_service.hpp"

#include "basic_value.hpp"
#include "service/log_service.hpp"

namespace service_delight {
    void BucketService::init() {
        Logger::get_instance().log(ConsoleLogger | BasicLogger, "Enter BucketService::init");
        if (!service_delight::StorageService::get_instance().init_flag()) {
            throw std::runtime_error("storage service not init");
        }
        bucket_collection = MongoProvider::get_instance().get_collection(schema::key::collection::bucket.data());
    }

    auto BucketService::add_one(const bsoncxx::oid& source_id, const std::string_view& bucket_name)
            -> schema::result<bsoncxx::oid, std::string_view> {
        Logger::get_instance().log(ConsoleLogger, "Enter BucketService::add_one_to_db");

        schema::DbBucket bucket;
        bucket.bucket_name = bucket_name;
        bucket.data_source = source_id;
        try {
            const auto insert_result = bucket_collection.insert_one(bucket.get_document());
            if (!insert_result) {
                throw std::runtime_error("BucketService::add_one: insert bucket failed");
            }
            return std::make_pair(insert_result->inserted_id().get_oid().value, "");
        }
        catch (const std::exception& e) {
            Logger::get_instance().log(
                    ConsoleLogger | BasicLogger, spdlog::level::err, "BucketService::add_one: insert bucket failed");
            return std::make_pair(std::nullopt, e.what());
        }
    }

    auto BucketService::add_one(const std::string_view& source_name, const std::string_view& bucket_name)
            -> schema::result<bsoncxx::oid, std::string_view> {
        Logger::get_instance().log(ConsoleLogger, "Enter BucketService::add_one");

        try {
            const auto [source_document, error] =
                    StorageService::get_instance().get_storage_by_name(source_name.data());
            if (not source_document.has_value()) {
                throw std::runtime_error(error.data());
            }
            const auto       source_id = source_document->find(schema::key::bson_id)->get_oid().value;
            schema::DbBucket bucket{};
            bucket.bucket_name = bucket_name;
            bucket.data_source = source_id;

            return add_one(source_id, bucket_name);
        }
        catch (const std::exception& e) {
            Logger::get_instance().log(ConsoleLogger | BasicLogger,
                                       spdlog::level::err,
                                       "BucketService::add_one: get storage failed {}",
                                       e.what());
            return std::make_pair(std::nullopt, e.what());
        }
    }

    auto BucketService::remove_one(const bsoncxx::oid& bucket_id) -> schema::result<bool, std::string_view> {
        Logger::get_instance().log(ConsoleLogger, "Enter BucketService::remove_from_db");
        try {
            const auto remove_result =
                    bucket_collection.delete_one(make_document(kvp(schema::key::bucket_id, bucket_id)));
            if (!remove_result) {
                throw std::runtime_error("BucketService::remove_from_db: remove bucket failed");
            }
            return std::make_pair(true, "");
        }
        catch (const std::exception& e) {
            Logger::get_instance().log(ConsoleLogger | BasicLogger,
                                       spdlog::level::err,
                                       "BucketService::remove_from_db: remove bucket failed");
            return std::make_pair(std::nullopt, e.what());
        }
    }

    auto BucketService::remove_one(const std::string_view& bucket_name) -> schema::result<bool, std::string_view> {
        Logger::get_instance().log(ConsoleLogger, "Enter BucketService::remove_from_db");
        try {
            const auto remove_result =
                    bucket_collection.delete_one(make_document(kvp(schema::key::bucket_name, bucket_name)));
            if (!remove_result) {
                throw std::runtime_error("BucketService::remove_from_db: remove bucket failed");
            }
            return std::make_pair(true, "");
        }
        catch (const std::exception& e) {
            Logger::get_instance().log(ConsoleLogger | BasicLogger,
                                       spdlog::level::err,
                                       "BucketService::remove_from_db: remove bucket failed");
            return std::make_pair(std::nullopt, e.what());
        }
    }

    auto BucketService::is_exist(const bsoncxx::oid& bucket_id) -> schema::result<bool, std::string_view> {
        Logger::get_instance().log(ConsoleLogger, "Enter BucketService::is_exist");
        try {
            const auto find_result = bucket_collection.find_one(make_document(kvp(schema::key::bucket_id, bucket_id)));
            if (!find_result) {
                throw std::runtime_error("BucketService::is_exist: find bucket failed");
            }
            return std::make_pair(find_result.has_value(), "");
        }
        catch (const std::exception& e) {
            Logger::get_instance().log(
                    ConsoleLogger | BasicLogger, spdlog::level::err, "BucketService::is_exist: find bucket failed");
            return std::make_pair(std::nullopt, e.what());
        }
    }

    auto BucketService::is_exist(const std::string_view& bucket_name) -> schema::result<bool, std::string_view> {
        Logger::get_instance().log(ConsoleLogger, "Enter BucketService::is_exist");
        try {
            const auto find_result =
                    bucket_collection.find_one(make_document(kvp(schema::key::bucket_name, bucket_name)));
            if (!find_result) {
                throw std::runtime_error("BucketService::is_exist: find bucket failed");
            }
            return std::make_pair(find_result.has_value(), "");
        }
        catch (const std::exception& e) {
            Logger::get_instance().log(
                    ConsoleLogger | BasicLogger, spdlog::level::err, "BucketService::is_exist: find bucket failed");
            return std::make_pair(std::nullopt, e.what());
        }
    }

}  // namespace service_delight
