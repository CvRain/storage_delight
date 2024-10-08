//
// Created by cvrain on 24-10-8.
//

#ifndef LOG_SERVICE_HPP
#define LOG_SERVICE_HPP

#include <mongocxx/collection.hpp>

#include "utils/singleton_prototype.hpp"
#include "models/schema_key.hpp"

#include "service/logger.hpp"


namespace service_delight {
    class LogService : public util_delight::Singleton<LogService> {
    public:
        void init();
        void record_operation(schema::DbOperationLog*  operation_log);
        schema::result<nlohmann::json, std::string> get_operation_by_id(const bsoncxx::oid& record_id);
        schema::result<nlohmann::json, std::string>  get_operation_by_user_id(const bsoncxx::oid& user_id);

    private:
        mongocxx::collection log_collection;
    };

} // namespace service_delight

#endif // LOG_SERVICE_HPP
