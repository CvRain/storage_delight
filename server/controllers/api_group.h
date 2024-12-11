#pragma once

#include <drogon/HttpController.h>

#include "basic_value.hpp"
#include "models/nlohmann_json_request.hpp"

using namespace drogon;

namespace api {
    class Group final : public drogon::HttpController<Group> {
    public:
        // todo: add middleware
        METHOD_LIST_BEGIN
        METHOD_ADD(
                Group::add_member, "/members/add", Options, Patch, model_delight::basic_value::middleware::UserExist);

        METHOD_ADD(Group::remove_member,
                   "/members/remove",
                   Options,
                   Delete,
                   model_delight::basic_value::middleware::UserExist);

        METHOD_ADD(Group::add_bucket,
                   "/buckets/add",
                   Options,
                   Patch, Post,
                   model_delight::basic_value::middleware::UserExist,
                   model_delight::basic_value::middleware::GroupExist,
                   model_delight::basic_value::middleware::SourceExist);

        METHOD_ADD(Group::remove_bucket,
                   "/buckets/remove",
                   Options,
                   Delete, Post,
                   model_delight::basic_value::middleware::UserExist,
                   model_delight::basic_value::middleware::GroupExist,
                   model_delight::basic_value::middleware::SourceExist,
                   model_delight::basic_value::middleware::GroupOwner);

        METHOD_ADD(Group::list_bucket,
                   "/buckets/list",
                   Options,
                   Get,
                   model_delight::basic_value::middleware::UserExist,
                   model_delight::basic_value::middleware::GroupExist,
                   model_delight::basic_value::middleware::GroupOwner);

        METHOD_LIST_END
        static void add_member(model_delight::NlohmannJsonRequestPtr        &&req,
                               std::function<void(const HttpResponsePtr &)> &&callback);

        static void remove_member(model_delight::NlohmannJsonRequestPtr        &&req,
                                  std::function<void(const HttpResponsePtr &)> &&callback);

        static void add_bucket(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);

        static void remove_bucket(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);

        static void list_bucket(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
    };
}  // namespace api
