//
// Created by cvrain on 24-9-1.
//

#ifndef STORAGE_DELIGHT_NLOHMANN_JSON_REQUEST_HPP
#define STORAGE_DELIGHT_NLOHMANN_JSON_REQUEST_HPP

#include <nlohmann/json.hpp>
#include <drogon/HttpRequest.h>

namespace model_delight{
    class TestRequest: public drogon::HttpRequest {
    public:
        [[nodiscard]] nlohmann::json getNlohmannJsonBody() const{
            const auto json_value = drogon::HttpRequest::getBody();
            return nlohmann::json::parse(json_value);
        }
    };
    using TestRequestPtr = std::shared_ptr<TestRequest>;

    class NlohmannJsonRequest{
    public:
        explicit NlohmannJsonRequest(const drogon::HttpRequest &request)
                : m_request(request) {
        }

        const drogon::HttpRequest& operator-> () const{
            return m_request;
        }

        [[nodiscard]] const drogon::HttpRequest& getRequest() const{
            return m_request;
        }

        [[nodiscard]] nlohmann::json  getNlohmannJsonBody() const{
            return nlohmann::json::parse(m_request.getBody());
        }

    private:
        const drogon::HttpRequest &m_request;
    };

    using NlohmannJsonRequestPtr = std::shared_ptr<NlohmannJsonRequest>;
}
#endif //STORAGE_DELIGHT_NLOHMANN_JSON_REQUEST_HPP
