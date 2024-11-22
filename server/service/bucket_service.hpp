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
        auto add_one(const bsoncxx::oid& source_id, const std::string_view& bucket_name)
                -> schema::result<bsoncxx::oid, std::string_view>;
        auto add_one(const std::string_view& source_name, const std::string_view& bucket_name)
                -> schema::result<bsoncxx::oid, std::string_view>;
        auto remove_one(const bsoncxx::oid& bucket_id) -> schema::result<bool, std::string_view>;
        auto remove_one(const std::string_view& bucket_name) -> schema::result<bool, std::string_view>;
        auto is_exist(const bsoncxx::oid& bucket_id) -> schema::result<bool, std::string_view>;
        auto is_exist(const std::string_view& bucket_name) -> schema::result<bool, std::string_view>;
        auto list() -> schema::result<std::vector<bsoncxx::document::value>, std::string_view>;
        auto list(const bsoncxx::oid& id) -> schema::result<std::vector<bsoncxx::document::value>, std::string_view>;

    private:
        mongocxx::collection bucket_collection;
    };

}  // namespace service_delight


#endif  // BUCKET_SERVICE_HPP
