//
// Created by cvrain on 24-11-13.
//

#include "bucket_service.hpp"

#include "basic_value.hpp"
#include "service/log_service.hpp"

namespace service_delight {
    void BucketService::init() {
        Logger::get_instance().log(ConsoleLogger | BasicLogger, "Enter BucketService::init");
#ifndef STORAGE_SERVICE_HPP
        throw std::runtime_error("storage service not found");
#endif
        if (!service_delight::StorageService::get_instance().init_flag()) {
            throw std::runtime_error("storage service not init");
        }
        bucket_collection = MongoProvider::get_instance().get_collection(schema::key::collection::bucket.data());
    }

    auto BucketService::check_bucket_exist(const std::string& bucket_name) -> schema::result<bool, std::string_view> {
        Logger::get_instance().log(ConsoleLogger | BasicLogger, "Enter BucketService::check_bucket_exist");

        //todo
        return std::make_pair(true, "");
    }
}  // namespace service_delight
