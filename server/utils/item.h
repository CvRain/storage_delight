//
// Created by cvrain on 24-10-22.
//

#ifndef ITEM_H
#define ITEM_H

#include <drogon/HttpRequest.h>
#include <string>

#include "basic_value.hpp"

namespace util_delight {
    class Item {
    public:
        static std::string get_request_ip(const drogon::HttpRequestPtr &request) {
            if (const auto x_forward_for = request->getHeader(model_delight::basic_value::header::x_forwarded_for);
                !x_forward_for.empty()) {
                return x_forward_for;
            }
            return request->getPeerAddr().toIp();
        }
    };
} // namespace util_delight

#endif // ITEM_H
