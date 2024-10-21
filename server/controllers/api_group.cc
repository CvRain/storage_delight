#include "api_group.h"

#include "basic_value.hpp"
#include "nlohmann_json_response.hpp"
#include "service/group_service.hpp"
#include "service/log_service.hpp"
#include "service/logger.hpp"
#include "service/user_service.hpp"

using namespace api;

// Add definition of your processing function here
void Group::add_member(model_delight::NlohmannJsonRequestPtr &&req,
                       std::function<void(const HttpResponsePtr &)> &&callback) {
    const auto json_body = req->getNlohmannJsonBody();

    //check request key exist
    const auto request_exist = json_body.contains(schema::key::user_id)
    && json_body.contains(schema::key::group_id)
    && json_body.contains(schema::key::members_id);
    if(!request_exist) {
        nlohmann::json response{
            {model_delight::basic_value::request::code, k400BadRequest},
            {model_delight::basic_value::request::message, "Invalid request"},
            {model_delight::basic_value::request::result, "Invalid request"},
            {model_delight::basic_value::request::data, {}}
        };
        callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));
    }

    try {
        const auto user_id = bsoncxx::oid{json_body[schema::key::user_id].get<std::string>()};
        const auto group_id = bsoncxx::oid{json_body[schema::key::group_id].get<std::string>()};

        const auto members_group = json_body[schema::key::members_id].get<std::vector<std::string>>();
        std::vector<bsoncxx::oid> member_ids;
        member_ids.reserve(members_group.size());
        std::ranges::for_each(members_group, [&member_ids](const auto& member_id) {
            member_ids.emplace_back(bsoncxx::oid{member_id});
        });

    }catch (const std::exception& e) {
            nlohmann::json response{
            {model_delight::basic_value::request::code, k500InternalServerError},
            {model_delight::basic_value::request::message, "Failed to add member"},
            {model_delight::basic_value::request::result, e.what()},
            {model_delight::basic_value::request::data, {}}
        };
        callback(model_delight::NlohmannResponse::new_nlohmann_json_response(std::move(response)));

        service_delight::Logger::get_instance().log(service_delight::ConsoleLogger | service_delight::BasicLogger, spdlog::level::err,
            "Failed to add member due to invalid request {}", e.what());
    }


    //todo
    callback(HttpResponse::newHttpJsonResponse({}));
}

void Group::remove_member(model_delight::NlohmannJsonRequestPtr &&req,
                          std::function<void(const HttpResponsePtr &)> &&callback) {
    //todo
    callback(HttpResponse::newHttpJsonResponse({}));
}

