//
// Created by cvrain on 24-11-28.
//

#ifndef EXCEPTION_HANDLER_HPP
#define EXCEPTION_HANDLER_HPP

#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>

using namespace drogon;

namespace exception {

    class ExceptionHandler {
    public:
        static void handle(const HttpRequestPtr                          &request,
                           std::function<void(const HttpResponsePtr &)> &&callback,
                           const std::exception                          &exception);

        static void nlohmann_exception(const HttpRequestPtr                          &request,
                                       std::function<void(const HttpResponsePtr &)> &&callback,
                                       const std::exception                          &exception);

        static void base_exception(const HttpRequestPtr                          &request,
                                   std::function<void(const HttpResponsePtr &)> &&callback,
                                   const std::exception                          &exception);

        static void standard_exception(const HttpRequestPtr                          &request,
                                       std::function<void(const HttpResponsePtr &)> &&callback,
                                       const std::exception                          &exception);
    };

}  // namespace exception

#endif  // EXCEPTION_HANDLER_HPP
