#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

namespace api
{
class Object : public drogon::HttpController<Object>
{
  public:
    METHOD_LIST_BEGIN
    METHOD_ADD(Object::upload, "/upload", Post);

    METHOD_LIST_END
    // your declaration of processing function maybe like this:
    // void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int p1, std::string p2);
    // void your_method_name(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, double p1, int p2) const;
    static void upload(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback) ;
};
}
