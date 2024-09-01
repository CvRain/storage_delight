#ifndef DROGON_SPECIALIZATION_HPP
#define DROGON_SPECIALIZATION_HPP

#include <drogon/HttpRequest.h>
#include "nlohmann_json_request.hpp"

namespace drogon{
    template<>
    inline model_delight::NlohmannJsonRequest fromRequest(const HttpRequest& request){
        return {};
    }

    template<>
    inline model_delight::NlohmannJsonRequestPtr fromRequest(const HttpRequest& request){
        return std::make_shared<model_delight::NlohmannJsonRequest>();
    }
}


#endif // DROGON_SPECIALIZATION_HPP