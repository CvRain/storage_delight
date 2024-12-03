#pragma once

#include <drogon/HttpController.h>

#include "basic_value.hpp"

using namespace drogon;

namespace api {
    class Object final : public HttpController<Object> {
    public:
        METHOD_LIST_BEGIN
        METHOD_ADD(Object::upload,
                   "/upload",
                   Options,
                   Post,
                   model_delight::basic_value::middleware::UserExist,
                   model_delight::basic_value::middleware::SourceExist,
                   model_delight::basic_value::middleware::BucketExist);
        METHOD_ADD(Object::download,
                   "/download",
                   Options,
                   Get,
                   model_delight::basic_value::middleware::UserExist,
                   model_delight::basic_value::middleware::SourceExist,
                   model_delight::basic_value::middleware::BucketExist);
        METHOD_ADD(Object::list_objects,
                   "/list",
                   Options,
                   Get,
                   model_delight::basic_value::middleware::UserExist,
                   model_delight::basic_value::middleware::SourceExist,
                   model_delight::basic_value::middleware::BucketExist);
        METHOD_ADD(Object::remove_objects,
                   "/remove",
                   Options,
                   Delete,
                   model_delight::basic_value::middleware::UserExist,
                   model_delight::basic_value::middleware::SourceExist,
                   model_delight::basic_value::middleware::BucketExist);
        METHOD_ADD(Object::rename,
                   "/rename",
                   Options,
                   Patch,
                   model_delight::basic_value::middleware::UserExist,
                   model_delight::basic_value::middleware::SourceExist,
                   model_delight::basic_value::middleware::BucketExist);
        METHOD_LIST_END

        static void upload(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
        static void download(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
        static void list_objects(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
        static void remove_objects(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
        static void rename(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
    };
}  // namespace api
