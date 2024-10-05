//
// Created by cvrain-thinkbook on 24-8-18.
//

#ifndef STORAGE_DELIGHT_USER_FILTER_HPP
#define STORAGE_DELIGHT_USER_FILTER_HPP

#include <drogon/HttpFilter.h>
#include <drogon/HttpMiddleware.h>

#include "nlohmann_json_request.hpp"


namespace drogon::middleware {
    // class LoginMiddleware : public HttpMiddleware<LoginMiddleware> {
    // public:
    //     LoginMiddleware() = default;
    //
    //     static void invoke(model_delight::NlohmannJsonRequestPtr &&request, MiddlewareNextCallback &&nextCb, MiddlewareCallback &&mcb);
    // };

} // namespace drogon::middleware

#endif // STORAGE_DELIGHT_USER_FILTER_HPP
