//
// Created by cvrain on 24-9-1.
//

#ifndef STORAGE_DELIGHT_NLOHMANN_JSON_REQUEST_HPP
#define STORAGE_DELIGHT_NLOHMANN_JSON_REQUEST_HPP

#include <nlohmann/json.hpp>
#include <drogon/HttpRequest.h>

namespace model_delight{
    class NlohmannJsonRequest{

    };

    using NlohmannJsonRequestPtr = std::shared_ptr<NlohmannJsonRequest>;
}
#endif //STORAGE_DELIGHT_NLOHMANN_JSON_REQUEST_HPP
