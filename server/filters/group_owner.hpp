//
// Created by cvrain on 24-11-27.
//

#ifndef GROUP_OWNER_HPP
#define GROUP_OWNER_HPP

#include <drogon/HttpMiddleware.h>

/**
 * @brief 检查请求中的用户是否是组长
 */

namespace drogon::middleware {
    class GroupOwner final: public HttpMiddleware<GroupOwner> {
    public:
        void invoke(const HttpRequestPtr& req, MiddlewareNextCallback&& nextCb, MiddlewareCallback&& mcb) override;
    };
}  // namespace drogon::middleware


#endif //GROUP_OWNER_HPP
