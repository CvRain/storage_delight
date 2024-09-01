//
// Created by cvrain on 24-9-1.
//

#ifndef STORAGE_DELIGHT_NLOHMANN_JSON_REQUEST_HPP
#define STORAGE_DELIGHT_NLOHMANN_JSON_REQUEST_HPP

#include <nlohmann/json.hpp>
#include <drogon/HttpRequest.h>

namespace model_delight {
    class NlohmannJsonRequest{
    public:
        explicit NlohmannJsonRequest(const drogon::HttpRequest &request)
                : m_request(request) {
        }

        const drogon::HttpRequest& operator-> (){
            return m_request;
        }

        const drogon::HttpRequest& getRequest(){
            return m_request;
        }

        nlohmann::json  getNlohmannJsonBody(){
            return nlohmann::json::parse(m_request.getBody());
        }

    private:
        const drogon::HttpRequest &m_request;
    };

    using NlohmannJsonRequestPtr = std::shared_ptr<NlohmannJsonRequest>;
}
#endif //STORAGE_DELIGHT_NLOHMANN_JSON_REQUEST_HPP
