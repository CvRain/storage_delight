//
// Created by cvrain on 24-11-13.
//

#ifndef BUCKET_SERVICE_HPP
#define BUCKET_SERVICE_HPP

#include "service/mongo_service.hpp"
#include "service/storage_service.hpp"
#include "utils/singleton_prototype.hpp"

namespace service_delight {
    class BucketService : public util_delight::Singleton<BucketService> {
    public:
        ~    BucketService() = default;
        void init();
        auto check_bucket_exist(const std::string &bucket_name) -> schema::result<bool, std::string_view>;

    private:
        mongocxx::collection bucket_collection;
    };

}  // namespace service_delight


#endif  // BUCKET_SERVICE_HPP
