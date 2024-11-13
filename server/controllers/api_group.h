#pragma once

#include <drogon/HttpController.h>

#include "models/nlohmann_json_request.hpp"

using namespace drogon;

namespace api {
    class Group final : public drogon::HttpController<Group> {
    public:
        //todo: add middleware
        METHOD_LIST_BEGIN
        METHOD_ADD(Group::add_member, "/members/add", Options,Patch);
        METHOD_ADD(Group::remove_member, "/members/remove", Options,Delete);


        METHOD_LIST_END
        static void add_member(model_delight::NlohmannJsonRequestPtr &&req,
                        std::function<void(const HttpResponsePtr &)> &&callback);

        static void remove_member(model_delight::NlohmannJsonRequestPtr &&req,
                           std::function<void(const HttpResponsePtr &)> &&callback);
    };
} // namespace api
