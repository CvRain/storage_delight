//
// Created by cvrain on 24-8-8.
//

#ifndef STORAGE_DELIGHT_BASIC_TYPES_HPP
#define STORAGE_DELIGHT_BASIC_TYPES_HPP

#include <string>

namespace storage_delight::types {
    using file_content = struct {
        std::string_view name;
        const char* content;
        size_t size;
    };
}


#endif //STORAGE_DELIGHT_BASIC_TYPES_HPP
