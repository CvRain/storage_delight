//
// Created by cvrain on 24-7-22.
//

#ifndef STORAGE_DELIGHT_HELLO_HPP
#define STORAGE_DELIGHT_HELLO_HPP

#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/json.h>
#include <iostream>
#include <string>

using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams

namespace storage_delight::core {
    class Hello{
    public:
        static pplx::task<void> say_hello();
    };
}

#endif //STORAGE_DELIGHT_HELLO_HPP
