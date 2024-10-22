#include "api_group.h"

#include "basic_value.hpp"
#include "nlohmann_json_response.hpp"
#include "service/group_service.hpp"
#include "service/log_service.hpp"
#include "service/logger.hpp"
#include "service/user_service.hpp"
#include "utils/item.h"

using namespace api;

void Group::add_member(model_delight::NlohmannJsonRequestPtr &&req,
                       std::function<void(const HttpResponsePtr &)> &&callback) {
    service_delight::Logger::get_instance().log(service_delight::ConsoleLogger, spdlog::level::debug,
                                                "Enter Group::add_member");

    const auto json_body = req->getNlohmannJsonBody();

    auto mongo_session = service_delight::MongoProvider::get_instance().start_session();
    mongo_session.start_transaction();
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
                    service_delight::ConsoleLogger, spdlog::level::debug,
                    "Group::add_member: Failed to add member: request key not exist");
            return;
        }

        const auto user_id = bsoncxx::oid{json_body.at(schema::key::user_id).get<std::string>()};
        service_delight::Logger::get_instance().log(service_delight::ConsoleLogger, spdlog::level::debug,
                                                    "Request user id: {}",
                                                    json_body.at(schema::key::user_id).get<std::string>());

        // 检查user_id是否存在
        if (!service_delight::UserService::get_instance().user_is_exist(user_id)) {
            nlohmann::json response{{model_delight::basic_value::request::code, k400BadRequest},
                                    {model_delight::basic_value::request::message, "Invalid request"},
                                    {model_delight::basic_value::request::result, "group owner not exist"},
                                    {model_delight::basic_value::request::data, {}}};
            callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));
            service_delight::Logger::get_instance().log(
                    service_delight::ConsoleLogger, spdlog::level::debug,
                    "Group::add_member: Failed to add member: group owner not exist");
            return;
        }

        const auto group_id = bsoncxx::oid{json_body.at(schema::key::group_id).get<std::string>()};
        service_delight::Logger::get_instance().log(service_delight::ConsoleLogger, spdlog::level::debug,
                                                    "Request group id: {}",
                                                    json_body.at(schema::key::group_id).get<std::string>());

        // 检查group是否存在
        if (const auto [result, error] = service_delight::GroupService::get_instance().group_exist(group_id);
            !result.has_value() || result.value() == false) {
            nlohmann::json response{{model_delight::basic_value::request::code, k400BadRequest},
                                    {model_delight::basic_value::request::message, "Invalid request"},
                                    {model_delight::basic_value::request::result, "group not exist"},
                                    {model_delight::basic_value::request::data, {}}};
            callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));
            service_delight::Logger::get_instance().log(service_delight::ConsoleLogger, spdlog::level::debug,
                                                        "Group::add_member: Failed to add member: group not exist");
            return;
        }

        const auto members_group = json_body[schema::key::members_id].get<std::vector<std::string>>();
        std::vector<bsoncxx::oid> member_ids;
        member_ids.reserve(members_group.size());

        // 检查user_id是否是group_id的owner
        if (const auto [result, error] = service_delight::GroupService::get_instance().get_group_owner(group_id);
            !result.has_value() || result.value() != user_id) {
            nlohmann::json response{
                    {model_delight::basic_value::request::code, k400BadRequest},
                    {model_delight::basic_value::request::message, "Invalid request"},
                    {model_delight::basic_value::request::result, "group owner is not the same as this user"},
                    {model_delight::basic_value::request::data, {}}};
            callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));
            service_delight::Logger::get_instance().log(
                    service_delight::ConsoleLogger, spdlog::level::debug,
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
        if (std::ranges::any_of(member_ids, [](const auto &member_id) {
                return !service_delight::UserService::get_instance().user_is_exist(member_id);
            })) {
            nlohmann::json response{{model_delight::basic_value::request::code, k400BadRequest},
                                    {model_delight::basic_value::request::message, "Invalid request"},
                                    {model_delight::basic_value::request::result, "user not exist"},
                                    {model_delight::basic_value::request::data, {}}};
            callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));
            service_delight::Logger::get_instance().log(service_delight::ConsoleLogger, spdlog::level::debug,
                                                        "Group::add_member: Failed to add member: user not exist");
            return;
        }

        // 检查添加用户操作是否成功
        const auto [result, error] = service_delight::GroupService::get_instance().add_members(group_id, member_ids);
        mongo_session.commit_transaction();

        if (result.has_value()) {
            nlohmann::json response{{model_delight::basic_value::request::code, k200OK},
                                    {model_delight::basic_value::request::message, "Successfully added member"},
                                    {model_delight::basic_value::request::result, "Successfully added member"},
                                    {model_delight::basic_value::request::data, {}}};
            // service_delight::MongoProvider::commit_transaction(mongo_session);


            callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));
            service_delight::Logger::get_instance().log(service_delight::ConsoleLogger, spdlog::level::debug,
                                                        "Group::add_member: Successfully added member");
        }
        else {
            nlohmann::json response{{model_delight::basic_value::request::code, k500InternalServerError},
                                    {model_delight::basic_value::request::message, "Failed to add member"},
                                    {model_delight::basic_value::request::result, error},
                                    {model_delight::basic_value::request::data, {}}};
            callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));

            // service_delight::MongoProvider::abort_transaction(mongo_session);
            mongo_session.abort_transaction();

            service_delight::Logger::get_instance().log(
                    service_delight::ConsoleLogger, spdlog::level::debug,
                    "Group::add_member: Failed to add member: failed to add member");
        }
    }
    catch (const std::exception &e) {
        nlohmann::json response{{model_delight::basic_value::request::code, k500InternalServerError},
                                {model_delight::basic_value::request::message, "Failed to add member"},
                                {model_delight::basic_value::request::result, e.what()},
                                {model_delight::basic_value::request::data, {}}};
        callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));
        service_delight::MongoProvider::abort_transaction(mongo_session);

        service_delight::Logger::get_instance().log(service_delight::ConsoleLogger | service_delight::BasicLogger,
                                                    spdlog::level::err,
                                                    "Failed to add member due to invalid request {}", e.what());
    }
}

void Group::remove_member(model_delight::NlohmannJsonRequestPtr &&req,
                          std::function<void(const HttpResponsePtr &)> &&callback) {
    //todo
    callback(HttpResponse::newHttpJsonResponse({}));
}

