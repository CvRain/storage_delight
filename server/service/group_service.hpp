//
// Created by cvrain on 24-10-5.
//

#ifndef GROUP_SERVICE_HPP
#define GROUP_SERVICE_HPP

#include "models/db_schema.hpp"
#include "mongo_service.hpp"
#include "type.hpp"
#include "utils/singleton_prototype.hpp"

namespace service_delight {

    class GroupService : public util_delight::Singleton<GroupService> {
    public:
        void init();
        schema::result<std::string, std::string> add_group(schema::DbGroup *group);
        schema::result<schema::DbGroup, std::string> get_group(const bsoncxx::oid &group_id);
        schema::result<bool, std::string> group_exist(const bsoncxx::oid &group_id);
    private:
        mongocxx::collection group_collection;
    };

} // service_delight

#endif //GROUP_SERVICE_HPP
