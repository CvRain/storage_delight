//
// Created by cvrain on 24-11-10.
//

#ifndef STORAGE_SERVICE_HPP
#define STORAGE_SERVICE_HPP

#include "client.hpp"
#include "models/db_schema.hpp"
#include "service/mongo_service.hpp"
#include "type.hpp"
#include "utils/singleton_prototype.hpp"

namespace service_delight {
    class StorageService final : public util_delight::Singleton<StorageService> {
    public:
        ~    StorageService() = default;
        auto init() -> void;

        auto init_flag() const -> const bool &;

        auto append_storage(schema::DbDataSource *data_source) -> schema::result<bsoncxx::oid, std::string_view>;

        auto get_storage_by_id(const bsoncxx::oid &id) -> schema::result<bsoncxx::document::value, std::string_view>;

        auto get_storage_by_name(const std::string_view &name) -> schema::result<bsoncxx::document::value, std::string_view>;

        auto list_all_storage_ids() -> schema::result<std::vector<bsoncxx::oid>, std::string_view>;

        auto list_all_storages() -> schema::result<std::vector<bsoncxx::document::value>, std::string_view>;

        auto remove_storage(const bsoncxx::oid &id) -> schema::result<bool, std::string_view>;

        auto update_storage(schema::DbDataSource *data_source) -> schema::result<bool, std::string_view>;

        auto is_exist(const bsoncxx::oid &id) -> schema::result<bool, std::string_view>;

        auto is_exist(const std::string &name) -> schema::result<bool, std::string_view>;

        auto active_storage(const bsoncxx::oid &id) -> schema::result<bool, std::string_view>;

        auto active_all_storage() -> schema::result<bool, std::string_view>;

        auto is_active(const bsoncxx::oid& id) -> schema::result<bool, std::string_view>;

        auto inactive_storage(const bsoncxx::oid &id) -> schema::result<bool, std::string_view>;

        auto inactive_all_storage() -> schema::result<bool, std::string_view>;

        auto source_name(const bsoncxx::oid& source_id) -> schema::result<std::string, std::string_view>;

        auto get_client(const bsoncxx::oid& source_id) -> std::optional<std::shared_ptr<storage_delight::core::Client>>;

        auto generate_client(const bsoncxx::oid& source_id) -> std::shared_ptr<storage_delight::core::Client>;

    private:
        mongocxx::collection data_source_collection;
        bool is_init = false;
        storage_delight::core::ClientGroupV2<bsoncxx::oid> client_group;
        mutable std::mutex mutex_{};
    };
} // namespace service_delight
#endif // STORAGE_SERVICE_HPP
