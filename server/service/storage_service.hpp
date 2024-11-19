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
        auto append_storage(schema::DbDataSource *data_source) -> schema::result<bool, std::string_view>;

        /**
         * @brief 通过id获取存储源
         * @param id 存储源的id
         * @return 如果查询成功返回查询后的document，失败则为std::nullopt和错误原因
         */
        auto get_storage_by_id(const bsoncxx::oid &id) -> schema::result<bsoncxx::document::value, std::string_view>;

        /**
         * @brief 通过名称获取存储源
         * @param name 存储源的名称
         * @return 如果查询成功返回查询后的document，失败则为std::nullopt和错误原因
         */
        auto get_storage_by_name(const std::string_view &name) -> schema::result<bsoncxx::document::value, std::string_view>;

        /**
         * @brief 列出所有存储源的id
         * @return 如果查询成功返回查询后的oid数组，失败则为std::nullopt和错误原因
         */
        auto list_all_storage_ids() -> schema::result<std::vector<bsoncxx::oid>, std::string_view>;

        /**
         * @brief 列出所有存储源
         * @return 如果查询成功返回查询后的document数组，失败则为std::nullopt和错误原因
         */
        auto list_all_storages() -> schema::result<std::vector<bsoncxx::document::value>, std::string_view>;

        /**
         * @brief 删除存储源
         * @param id 存储源的id
         * @return 如果删除成功返回true，失败则为false和错误原因
         */
        auto remove_storage(const bsoncxx::oid &id) -> schema::result<bool, std::string_view>;

        /**
         * @brief 更新存储源
         * @param data_source 接受DbDataSource结构
         * @return 如果更新成功返回true，失败则为false和错误原因
         */
        auto update_storage(schema::DbDataSource *data_source) -> schema::result<bool, std::string_view>;

        /**
         * @brief 检查存储源是否存在
         * @param id 存储源的id
         * @return 如果存在返回true，不存在返回false，失败则为false和错误原因
         */
        auto is_exist(const bsoncxx::oid &id) -> schema::result<bool, std::string_view>;

        /**
         * @brief 检查存储源是否存在
         * @param name 存储源的名称
         * @return 如果存在返回true，不存在返回false，失败则为false和错误原因
         */
        auto is_exist(const std::string &name) -> schema::result<bool, std::string_view>;

        auto active_storage(const bsoncxx::oid &id) -> schema::result<bool, std::string_view>;

        auto active_all_storage() -> schema::result<bool, std::string_view>;

        auto is_active(const std::string_view &source_name) -> schema::result<bool, std::string_view>;

        auto inactive_storage(const bsoncxx::oid &id) -> schema::result<bool, std::string_view>;

        auto inactive_all_storage() -> schema::result<bool, std::string_view>;

        auto get_client(const std::string_view& source_name) -> std::optional<std::shared_ptr<storage_delight::core::Client>>;

    private:
        mongocxx::collection data_source_collection;
        bool is_init = false;
        storage_delight::core::ClientGroup client_group;
    };
} // namespace service_delight
#endif // STORAGE_SERVICE_HPP
