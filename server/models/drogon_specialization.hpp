#ifndef DROGON_SPECIALIZATION_HPP
#define DROGON_SPECIALIZATION_HPP

#include <drogon/HttpRequest.h>
#include "nlohmann_json_request.hpp"
#include "nlohmann_json_response.hpp"

namespace drogon{
    template<>
    inline model_delight::NlohmannJsonRequestPtr fromRequest(const HttpRequest& request){
        return std::make_shared<model_delight::NlohmannJsonRequest>(request);
    }

    template<>
    inline HttpResponsePtr toResponse(nlohmann::json &&value){
        return model_delight::HttpResponse::newHttpNlohmannJsonResponse(std::move(value));
    }
}


#endif // DROGON_SPECIALIZATION_HPP