//
// Created by cvrain-thinkbook on 24-8-18.
//

#ifndef STORAGE_DELIGHT_USER_FILTER_HPP
#define STORAGE_DELIGHT_USER_FILTER_HPP

#include <drogon/HttpFilter.h>
#include <drogon/HttpMiddleware.h>


namespace drogon::middleware {

    const std::string LoginMiddlewareName = "drogon::middleware::LoginMiddleware";
    class LoginMiddleware: public HttpMiddleware<LoginMiddleware>{
    public:
        LoginMiddleware() = default;

        void invoke(const HttpRequestPtr &req,
                    MiddlewareNextCallback &&nextCb,
                    MiddlewareCallback &&mcb) override;
    };

} // filter

#endif //STORAGE_DELIGHT_USER_FILTER_HPP
