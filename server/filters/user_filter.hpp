//
// Created by cvrain-thinkbook on 24-8-18.
//

#ifndef STORAGE_DELIGHT_USER_FILTER_HPP
#define STORAGE_DELIGHT_USER_FILTER_HPP

#include <drogon/HttpFilter.h>
#include <drogon/HttpMiddleware.h>

namespace drogon {

    class UserFilter: public HttpMiddleware<UserFilter>{
    public:
        UserFilter() = default;
    };

} // filter

#endif //STORAGE_DELIGHT_USER_FILTER_HPP
