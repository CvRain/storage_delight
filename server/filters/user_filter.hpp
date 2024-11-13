//
// Created by cvrain-thinkbook on 24-8-18.
//

#ifndef STORAGE_DELIGHT_USER_FILTER_HPP
#define STORAGE_DELIGHT_USER_FILTER_HPP

#include <drogon/HttpMiddleware.h>

namespace drogon::middleware {
    class LoginMiddleware : public HttpMiddleware<LoginMiddleware> {
    public:
        LoginMiddleware() = default;

        void invoke(const HttpRequestPtr& request, MiddlewareNextCallback &&nextCb, MiddlewareCallback &&mcb) override;
    };

} // namespace drogon::middleware

#endif // STORAGE_DELIGHT_USER_FILTER_HPP
