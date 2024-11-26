//
// Created by cvrain on 24-11-26.
//

#ifndef GROUP_EXIST_HPP
#define GROUP_EXIST_HPP

#include <drogon/HttpMiddleware.h>

namespace drogon::middleware {
    class GroupExist final : public HttpMiddleware<GroupExist> {
    public:
        void invoke(const HttpRequestPtr& req, MiddlewareNextCallback&& nextCb, MiddlewareCallback&& mcb) override;
    };
}  // namespace drogon::middleware


#endif  // GROUP_EXIST_HPP
