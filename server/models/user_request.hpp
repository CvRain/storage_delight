#pragma once

#include <drogon/HttpRequest.h>
#include <json/json.h>
#include <string>
#include <map>

#include "db_schema.hpp"

namespace drogon{
    template<>
    schema::BaseUser fromRequest(const HttpRequest &request);
}