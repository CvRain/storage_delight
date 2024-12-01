#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

namespace api {
    class Object final : public HttpController<Object> {
    public:
        METHOD_LIST_BEGIN
        METHOD_ADD(Object::upload, "/upload", Options, Post);
        METHOD_ADD(Object::download, "/download", Options, Get);
        METHOD_LIST_END

        static void upload(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
        static void download(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
        static void list_objects(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
        static void remove_objects(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
    };
}  // namespace api
