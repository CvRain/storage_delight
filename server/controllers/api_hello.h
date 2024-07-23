#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

namespace api {
    class Hello : public drogon::HttpController<Hello> {
    public:
        METHOD_LIST_BEGIN
            METHOD_ADD(Hello::say, "/say", Get);

        METHOD_LIST_END

        void say(const HttpRequestPtr &req,
                   std::function<void(const HttpResponsePtr &)> &&callback);

    };
}
