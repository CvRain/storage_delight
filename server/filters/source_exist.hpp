//
// Created by cvrain on 24-11-26.
//

#ifndef SOURCE_EXIST_HPP
#define SOURCE_EXIST_HPP

#include <drogon/HttpMiddleware.h>

namespace drogon::middleware {
    class SourceExist final : public HttpMiddleware<SourceExist> {
    public:
        void invoke(const HttpRequestPtr& req, MiddlewareNextCallback&& nextCb, MiddlewareCallback&& mcb) override;
    };
}  // namespace drogon::middleware

#endif  // SOURCE_EXIST_HPP
