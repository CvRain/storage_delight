//
// Created by cvrain on 24-7-22.
//

#include "hello.hpp"

#include <iostream>

namespace storage_delight::core {

    pplx::task<void> Hello::say_hello() {
        web::http::client::http_client client(U("http://localhost:3000"));

        web::http::uri_builder builder(U("/api/hello/say"));

        const auto result = client.request(web::http::methods::GET, builder.to_string())
                .then([](const web::http::http_response &response) {
                    return response.extract_json();
                }).then([](const web::json::value &jsonValue) {
                    std::cout << "GET Response " << jsonValue.serialize() << std::endl;
                });

        return result;
    }
}