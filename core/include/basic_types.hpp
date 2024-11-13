//
// Created by cvrain on 24-8-8.
//

#ifndef STORAGE_DELIGHT_BASIC_TYPES_HPP
#define STORAGE_DELIGHT_BASIC_TYPES_HPP

#include <string>
#include <miniocpp/client.h>

namespace storage_delight::types {
    using file_content = struct {
        std::string_view name;
        const char* content;
        size_t size;
    };

    struct client_config {
        minio::s3::BaseUrl base_url;
        std::string access_key;
        std::string secret_key;
    };
}


#endif //STORAGE_DELIGHT_BASIC_TYPES_HPP
