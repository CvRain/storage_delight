//
// Created by cvrain on 24-11-17.
//

#ifndef USER_EXIST_HPP
#define USER_EXIST_HPP

#include <drogon/HttpMiddleware.h>
#include "drogon_specialization.hpp"

namespace drogon::middleware {
    class UserExist final : public HttpMiddleware<UserExist> {
    public:
             UserExist() = default;
        void invoke(const HttpRequestPtr &request, MiddlewareNextCallback &&nextCb, MiddlewareCallback &&mcb) override;
    };
}  // namespace drogon::middleware


#endif  // USER_EXIST_HPP
