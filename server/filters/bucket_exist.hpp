//
// Created by cvrain on 24-12-2.
//

#ifndef BUCKET_EXIST_HPP
#define BUCKET_EXIST_HPP

#include <drogon/HttpMiddleware.h>

namespace drogon::middleware {
    class BucketExist final : public HttpMiddleware<BucketExist> {
    public:
        void invoke(const HttpRequestPtr& req, MiddlewareNextCallback&& nextCb, MiddlewareCallback&& mcb) override;
    };
}  // namespace drogon::middleware


#endif  // BUCKET_EXIST_HPP
