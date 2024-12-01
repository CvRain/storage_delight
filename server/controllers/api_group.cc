#include "api_group.h"

#include "basic_value.hpp"
#include "nlohmann_json_response.hpp"
#include "service/group_service.hpp"
#include "service/log_service.hpp"
#include "service/logger.hpp"
#include "service/storage_service.hpp"
#include "service/user_service.hpp"
#include "utils/exception_handler.hpp"
#include "utils/item.h"

using namespace api;

void Group::add_member(model_delight::NlohmannJsonRequestPtr        &&req,
                       std::function<void(const HttpResponsePtr &)> &&callback) {
    service_delight::Logger::get_instance().log(
            service_delight::ConsoleLogger, spdlog::level::debug, "Enter Group::add_member");

    const auto json_body = req->getNlohmannJsonBody();

    try {
        // 检查字段是否存在
        const auto request_exist = json_body.contains(schema::key::user_id) &&
                                   json_body.contains(schema::key::group_id) &&
                                   json_body.contains(schema::key::members_id);
        if (!request_exist) {
            nlohmann::json response{{model_delight::basic_value::request::code, k400BadRequest},
                                    {model_delight::basic_value::request::message, "Invalid request"},
                                    {model_delight::basic_value::request::result, "Request key not exist"},
                                    {model_delight::basic_value::request::data, {}}};
            callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));

            service_delight::Logger::get_instance().log(
                    service_delight::ConsoleLogger,
                    spdlog::level::debug,
                    "Group::add_member: Failed to add member: request key not exist");
            return;
        }

        const auto user_id = bsoncxx::oid{json_body.at(schema::key::user_id).get<std::string>()};
        service_delight::Logger::get_instance().log(service_delight::ConsoleLogger,
                                                    spdlog::level::debug,
                                                    "Request user id: {}",
                                                    json_body.at(schema::key::user_id).get<std::string>());

        // 检查user_id是否存在
        service_delight::Logger::get_instance().log(
                service_delight::ConsoleLogger, spdlog::level::debug, "Group::add_member: Checking user_id existence");
        if (const auto &[user_exist_result, user_exist_error] =
                    service_delight::UserService::get_instance().is_exist(user_id);
            not user_exist_result.has_value() || user_exist_result.value() == false)
        {
            nlohmann::json response{{model_delight::basic_value::request::code, k400BadRequest},
                                    {model_delight::basic_value::request::message, "Invalid request"},
                                    {model_delight::basic_value::request::result, user_exist_error},
                                    {model_delight::basic_value::request::data, {}}};
            callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));
            service_delight::Logger::get_instance().log(
                    service_delight::ConsoleLogger, spdlog::level::debug, "Group::add_member: ", user_exist_error);
            return;
        }

        const auto group_id = bsoncxx::oid{json_body.at(schema::key::group_id).get<std::string>()};
        service_delight::Logger::get_instance().log(service_delight::ConsoleLogger,
                                                    spdlog::level::debug,
                                                    "Request group id: {}",
                                                    json_body.at(schema::key::group_id).get<std::string>());

        // 检查group是否存在
        if (const auto [result, error] = service_delight::GroupService::get_instance().is_exist(group_id);
            !result.has_value() || result.value() == false)
        {
            nlohmann::json response{{model_delight::basic_value::request::code, k400BadRequest},
                                    {model_delight::basic_value::request::message, "Invalid request"},
                                    {model_delight::basic_value::request::result, "group not exist"},
                                    {model_delight::basic_value::request::data, {}}};
            callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));
            service_delight::Logger::get_instance().log(service_delight::ConsoleLogger,
                                                        spdlog::level::debug,
                                                        "Group::add_member: Failed to add member: group not exist");
            return;
        }

        const auto                members_group = json_body[schema::key::members_id].get<std::vector<std::string>>();
        std::vector<bsoncxx::oid> member_ids;
        member_ids.reserve(members_group.size());

        const auto group_previous_state =
                service_delight::GroupService::get_instance().get_one_to_bson(group_id).first.value();

        // 检查user_id是否是group_id的owner
        if (const auto [result, error] = service_delight::GroupService::get_instance().get_owner(group_id);
            !result.has_value() || result.value() != user_id)
        {
            nlohmann::json response{
                    {model_delight::basic_value::request::code, k400BadRequest},
                    {model_delight::basic_value::request::message, "Invalid request"},
                    {model_delight::basic_value::request::result, "group owner is not the same as this user"},
                    {model_delight::basic_value::request::data, {}}};
            callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));
            service_delight::Logger::get_instance().log(
                    service_delight::ConsoleLogger,
                    spdlog::level::debug,
                    "Group::add_member: Failed to add member: group owner is not the same as this user");
            return;
        }

        for (const auto &member_id: members_group) {
            member_ids.emplace_back(member_id);
        }

        // 去除重复项
        std::ranges::sort(member_ids);
        member_ids.erase(std::ranges::unique(member_ids).begin(), member_ids.end());

        // 检查member_id中的用户是否存在
        for (const auto &member_id: member_ids) {
            if (const auto [user_exist_result, user_exist_error] =
                        service_delight::UserService::get_instance().is_exist(member_id);
                not user_exist_result.has_value() || user_exist_result.value() == false)
            {
                model_delight::BasicResponse response{.code    = k400BadRequest,
                                                      .message = "k400BadRequest",
                                                      .result  = member_id.to_string() + "not exist",
                                                      .data    = {}};
                callback(newHttpJsonResponse(std::move(response.to_json())));
                service_delight::Logger::get_instance().log(
                        service_delight::ConsoleLogger,
                        spdlog::level::debug,
                        "Group::add_member: Failed to add member: user {} not exist",
                        member_id.to_string());
                return;
            }
        }

        // 检查添加用户操作是否成功
        const auto [result, error] = service_delight::GroupService::get_instance().add_members(group_id, member_ids);

        if (result.has_value()) {
            nlohmann::json response{{model_delight::basic_value::request::code, k200OK},
                                    {model_delight::basic_value::request::message, "Successfully added member"},
                                    {model_delight::basic_value::request::result, "Successfully added member"},
                                    {model_delight::basic_value::request::data, {}}};


            callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));
            service_delight::Logger::get_instance().log(service_delight::ConsoleLogger,
                                                        spdlog::level::debug,
                                                        "Group::add_member: Successfully added member");
        }
        else {
            nlohmann::json response{{model_delight::basic_value::request::code, k500InternalServerError},
                                    {model_delight::basic_value::request::message, "Failed to add member"},
                                    {model_delight::basic_value::request::result, error},
                                    {model_delight::basic_value::request::data, {}}};
            callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));

            service_delight::Logger::get_instance().log(
                    service_delight::ConsoleLogger,
                    spdlog::level::debug,
                    "Group::add_member: Failed to add member: failed to add member");
        }

        const auto group_current_state =
                service_delight::GroupService::get_instance().get_one_to_bson(group_id).first.value();

        // 写入日志
        schema::DbOperationLog operation_log{};
        operation_log.action         = "add member to group";
        operation_log.current_state  = bsoncxx::to_json(group_current_state);
        operation_log.previous_state = bsoncxx::to_json(group_previous_state);
        operation_log.description    = error;
        operation_log.timestamp      = util_delight::Date::get_current_timestamp_32();
        operation_log.user_id        = user_id;
        operation_log.request_ip     = req->getRequest().getPeerAddr().toIp();

        service_delight::LogService::get_instance().record_operation(&operation_log);
    }
    catch (const std::exception &e) {
        nlohmann::json response{{model_delight::basic_value::request::code, k500InternalServerError},
                                {model_delight::basic_value::request::message, "Failed to add member"},
                                {model_delight::basic_value::request::result, e.what()},
                                {model_delight::basic_value::request::data, {}}};
        callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));
        service_delight::Logger::get_instance().log(service_delight::ConsoleLogger | service_delight::BasicLogger,
                                                    spdlog::level::err,
                                                    "Failed to add member due to invalid request {}",
                                                    e.what());
    }
}

void Group::remove_member(model_delight::NlohmannJsonRequestPtr        &&req,
                          std::function<void(const HttpResponsePtr &)> &&callback) {
    service_delight::Logger::get_instance().log(
            service_delight::ConsoleLogger, spdlog::level::debug, "Enter Group::remove_member");

    const auto json_body  = req->getNlohmannJsonBody();
    const auto request_ip = req->getRequest().getPeerAddr().toIp();

    auto mongo_session = service_delight::MongoProvider::get_instance().start_session();
    try {
        const auto field_user_id    = json_body.at(schema::key::user_id).get<std::string>();
        const auto field_group_id   = json_body.at(schema::key::group_id).get<std::string>();
        const auto field_members_id = json_body.at(schema::key::members_id).get<std::vector<std::string>>();

        // 检查用户是否存在
        const auto [user_exist_result, error] = service_delight::UserService::get_instance().is_exist(field_user_id);
        if (not user_exist_result.has_value()) {
            nlohmann::json response{{model_delight::basic_value::request::code, k500InternalServerError},
                                    {model_delight::basic_value::request::message, "k500InternalServerError"},
                                    {model_delight::basic_value::request::result, error},
                                    {model_delight::basic_value::request::data, {}}};
            callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));
            service_delight::Logger::get_instance().log(
                    service_delight::ConsoleLogger,
                    spdlog::level::debug,
                    "Group::remove_member: Failed to remove member: user not found");
            return;
        }

        if (user_exist_result.value() == false) {
            nlohmann::json response{{model_delight::basic_value::request::code, k400BadRequest},
                                    {model_delight::basic_value::request::message, "Invalid request"},
                                    {model_delight::basic_value::request::result, "User not found"},
                                    {model_delight::basic_value::request::data, {}}};
            callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));
            service_delight::Logger::get_instance().log(
                    service_delight::ConsoleLogger,
                    spdlog::level::debug,
                    "Group::remove_member: Failed to remove member: user not found");
            return;
        }

        // 获得当前状态的用户组信息
        auto [group_info, group_info_err] =
                service_delight::GroupService::get_instance().get_one(bsoncxx::oid{field_group_id});
        if (!group_info.has_value()) {
            nlohmann::json response{{model_delight::basic_value::request::code, k500InternalServerError},
                                    {model_delight::basic_value::request::message, "Failed to remove member"},
                                    {model_delight::basic_value::request::result, group_info_err},
                                    {model_delight::basic_value::request::data, {}}};
            callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));
            service_delight::Logger::get_instance().log(service_delight::ConsoleLogger,
                                                        spdlog::level::err,
                                                        "Group::remove_member: Failed to remove member: {}",
                                                        group_info_err);
            return;
        }
        const auto previous_state     = group_info.value().to_json();
        auto       current_group_info = group_info.value();

        // 检查用户组的owner_id是否是user_id
        if (current_group_info.owner_id != bsoncxx::oid{field_user_id}) {
            nlohmann::json response{{model_delight::basic_value::request::code, k400BadRequest},
                                    {model_delight::basic_value::request::message, "Invalid request"},
                                    {model_delight::basic_value::request::result, "User is not the owner"},
                                    {model_delight::basic_value::request::data, {}}};
            callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));
            service_delight::Logger::get_instance().log(
                    service_delight::ConsoleLogger,
                    spdlog::level::debug,
                    "Group::remove_member: Failed to remove member: user is not the owner");
            return;
        }

        // 获得 std::vector<bsoncxx::oid>格式的members_id
        std::vector<bsoncxx::oid> members_id;
        std::ranges::for_each(field_members_id,
                              [&members_id](const std::string &member_id) { members_id.emplace_back(member_id); });

        // 在group_info中查询是否存在待删除的成员,只要有一个不存在的就直接callback
        const auto all_contained = std::ranges::all_of(members_id, [&current_group_info](const bsoncxx::oid &id) {
            return std::ranges::find(current_group_info.members_id, id) != current_group_info.members_id.end();
        });
        if (!all_contained) {
            nlohmann::json response{{model_delight::basic_value::request::code, k400BadRequest},
                                    {model_delight::basic_value::request::message, "Invalid request"},
                                    {model_delight::basic_value::request::result, "Member not found"},
                                    {model_delight::basic_value::request::data, {}}};
            callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));
            service_delight::Logger::get_instance().log(
                    service_delight::ConsoleLogger,
                    spdlog::level::debug,
                    "Group::remove_member: Failed to remove member: member not found");
            return;
        }

        // 从group_info中删除成员
        auto filtered_view = current_group_info.members_id | std::views::filter([&members_id](const bsoncxx::oid &id) {
                                 return std::ranges::find(members_id, id) == members_id.end();
                             });
        current_group_info.members_id.assign(filtered_view.begin(), filtered_view.end());

        // 更新数据库信息
        const auto [update_result, update_error] =
                service_delight::GroupService::get_instance().update_one(&current_group_info);
        if (!update_result) {
            nlohmann::json response{{model_delight::basic_value::request::code, k500InternalServerError},
                                    {model_delight::basic_value::request::message, "Failed to remove member"},
                                    {model_delight::basic_value::request::result, update_error},
                                    {model_delight::basic_value::request::data, {}}};
            callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));
            service_delight::Logger::get_instance().log(service_delight::ConsoleLogger,
                                                        spdlog::level::err,
                                                        "Group::remove_member: Failed to remove member: {}",
                                                        update_error);
            return;
        }
        nlohmann::json response{{model_delight::basic_value::request::code, k200OK},
                                {model_delight::basic_value::request::message, "Successfully removed member"},
                                {model_delight::basic_value::request::result, "Success"},
                                {model_delight::basic_value::request::data, {}}};
        callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));
        service_delight::Logger::get_instance().log(service_delight::ConsoleLogger,
                                                    spdlog::level::info,
                                                    "Group::remove_member: Successfully removed member");

        schema::DbOperationLog operation_log{};
        operation_log.action         = "remove_member";
        operation_log.bucket_name    = current_group_info.name;
        operation_log.id             = bsoncxx::oid{field_user_id};
        operation_log.request_ip     = request_ip;
        operation_log.current_state  = current_group_info.to_json();
        operation_log.previous_state = previous_state;
        service_delight::LogService::get_instance().record_operation(&operation_log);
    }
    catch (const std::exception &e) {
        nlohmann::json response{{model_delight::basic_value::request::code, k500InternalServerError},
                                {model_delight::basic_value::request::message, "Failed to remove member"},
                                {model_delight::basic_value::request::result, e.what()},
                                {model_delight::basic_value::request::data, {}}};
        callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));
    }
}

// 感觉自己有点呆，在写remove_bucket的时候发现自己溜了一个remove_bucket的函数接口，但是没记得自己写。
// 跳转回去看了一眼发现自己add_bucket的接口也没有实现。突然冷汗出来了，那自己的这个controller怎么完成的。
// 看了一眼发现是直接获取了group在里面修改了然后update --cvrain 2024.11.28
void Group::add_bucket(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    service_delight::Logger::get_instance().log(
            service_delight::ConsoleLogger, spdlog::level::debug, "Enter Group::add_bucket");
    try {
        const auto &json_body   = fromRequest<nlohmann::json>(*req);
        const auto &user_id     = json_body.at(schema::key::user_id).get<std::string>();
        const auto &group_id    = json_body.at(schema::key::group_id).get<std::string>();
        const auto &source_id   = json_body.at(schema::key::source_id).get<std::string>();
        const auto &bucket_name = json_body.at(schema::key::bucket_name).get<std::string>();

        const auto &[find_group, find_group_error] =
                service_delight::GroupService::get_instance().get_one(bsoncxx::oid{group_id});

        auto        group                = find_group.value();
        const auto &group_previous_state = group.to_json().dump();

        // 检查bucket name是否存在于source中
        const auto client = service_delight::StorageService::get_instance().get_client(bsoncxx::oid{source_id});
        if (not client.has_value() || client.value() == nullptr) {
            throw exception::BaseException{model_delight::BasicResponse{.code    = k500InternalServerError,
                                                                        .message = "k500InternalServerError",
                                                                        .result  = "Failed to get client"}};
        }

        const auto bucket_list_response = client.value()->get_bucket_operation()->list_buckets();
        if (!bucket_list_response) {
            throw exception::BaseException{
                    model_delight::BasicResponse{.code    = k500InternalServerError,
                                                 .message = "k500InternalServerError",
                                                 .result  = bucket_list_response.Error().String()}};
        }
        const auto find_result = std::ranges::find_if(bucket_list_response.buckets, [&bucket_name](const auto &bucket) {
            return bucket.name == bucket_name;
        });

        if (find_result == bucket_list_response.buckets.end()) {
            throw exception::BaseException{model_delight::BasicResponse{
                    .code = k400BadRequest, .message = "Bucket not found", .result = "Bucket not found"}};
        }


        if (not std::ranges::any_of(group.buckets, [&](const auto &bucket) {
                return bucket.first == bsoncxx::oid{source_id} && bucket.second == bucket_name;
            }))
        {
            group.buckets.emplace_back(bsoncxx::oid{source_id}, bucket_name);
        }


        if (const auto &[update_result, update_error] =
                    service_delight::GroupService::get_instance().update_one(&group);
            not update_result.has_value())
        {
            service_delight::Logger::get_instance().log(service_delight::ConsoleLogger | service_delight::BasicLogger,
                                                        spdlog::level::err,
                                                        "Group::add_bucket: Failed to update group: {}",
                                                        update_error);
            throw exception::BaseException{model_delight::BasicResponse{.code    = k500InternalServerError,
                                                                        .message = "Failed to update group",
                                                                        .result  = update_error,
                                                                        .data    = {}}};
        }
        service_delight::Logger::get_instance().log(
                service_delight::ConsoleLogger, spdlog::level::info, "Group::add_bucket: Successfully added bucket");

        model_delight::BasicResponse response{
                .code = k200OK, .message = "k200OK", .result = "Success to add bucket", .data = group.to_json()};
        callback(newHttpJsonResponse(response.to_json()));

        schema::DbOperationLog operation_log{};
        operation_log.action         = "add_bucket";
        operation_log.bucket_name    = group.name;
        operation_log.current_state  = group.to_json().dump();
        operation_log.source_id      = bsoncxx::oid{source_id};
        operation_log.user_id        = bsoncxx::oid{user_id};
        operation_log.request_ip     = req->getPeerAddr().toIp();
        operation_log.previous_state = group_previous_state;
        service_delight::LogService::get_instance().record_operation(&operation_log);
    }
    catch (const std::exception &exception) {
        exception::ExceptionHandler::handle(req, std::move(callback), exception);
    }
}

void Group::remove_bucket(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    service_delight::Logger::get_instance().log(
                service_delight::ConsoleLogger, spdlog::level::debug, "Enter Group::remove_bucket");
    try {
        const auto &json_body   = fromRequest<nlohmann::json>(*req);
        const auto &user_id     = json_body.at(schema::key::user_id).get<std::string>();
        const auto &group_id    = json_body.at(schema::key::group_id).get<std::string>();
        const auto &source_id   = json_body.at(schema::key::source_id).get<std::string>();
        const auto &bucket_name = json_body.at(schema::key::bucket_name).get<std::string>();

        auto        group = service_delight::GroupService::get_instance().get_one(bsoncxx::oid{group_id}).first.value();
        const auto &group_previous_state = group.to_json().dump();

        // 使用 std::remove_if 和 vector::erase 来删除元素
        auto new_end = std::ranges::remove_if(group.buckets, [&](const auto &bucket) {
                           return bucket.first == bsoncxx::oid{source_id} && bucket.second == bucket_name;
                       }).begin();

        group.buckets.erase(new_end, group.buckets.end());

        if (const auto &[update_result, update_error] =
                    service_delight::GroupService::get_instance().update_one(&group);
            not update_result.has_value())
        {
            service_delight::Logger::get_instance().log(service_delight::ConsoleLogger | service_delight::BasicLogger,
                                                        spdlog::level::err,
                                                        "Group::remove_bucket: Failed to update group: {}",
                                                        update_error);
        }

        service_delight::Logger::get_instance().log(service_delight::ConsoleLogger,
                                                    spdlog::level::info,
                                                    "Group::remove_bucket: Successfully removed bucket");
        model_delight::BasicResponse response{
                .code = k200OK, .message = "k200OK", .result = "Success to remove bucket", .data = {}};
        callback(newHttpJsonResponse(response.to_json()));

        schema::DbOperationLog operation_log{};
        operation_log.action         = "remove_bucket";
        operation_log.bucket_name    = bucket_name;
        operation_log.current_state  = group.to_json().dump();
        operation_log.previous_state = group_previous_state;
        operation_log.source_id      = bsoncxx::oid{source_id};
        operation_log.user_id        = bsoncxx::oid{user_id};
        operation_log.request_ip     = req->getPeerAddr().toIp();
        service_delight::LogService::get_instance().record_operation(&operation_log);
    }
    catch (const std::exception &exception) {
        exception::ExceptionHandler::handle(req, std::move(callback), exception);
    }
}

void Group::list_bucket(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    service_delight::Logger::get_instance().log(
                service_delight::ConsoleLogger, spdlog::level::debug, "Enter Group::list_bucket");
    try {
        const auto &json_body = fromRequest<nlohmann::json>(*req);
        const auto &group_id  = json_body.at(schema::key::group_id).get<std::string>();

        const auto &group = service_delight::GroupService::get_instance().get_one(bsoncxx::oid{group_id}).first.value();
        nlohmann::json bucket_info{};
        for (const auto &[source_id, bucket_name]: group.buckets) {
            bucket_info.push_back(nlohmann::json{{schema::key::source_id, source_id.to_string()},
                    {schema::key::bucket_name, bucket_name}
                });
        }
        model_delight::BasicResponse response{
                .code = k200OK, .message = "k200OK", .result = "Success to list bucket", .data = bucket_info};
        callback(newHttpJsonResponse(response.to_json()));
    }
    catch (const std::exception &exception) {
        exception::ExceptionHandler::handle(req, std::move(callback), exception);
    }
}