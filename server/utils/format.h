#pragma once

#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/types/bson_value/view.hpp>
#include <string>

#include <nlohmann/json.hpp>

namespace util_delight {
    template <typename T>
    inline bsoncxx::builder::basic::array make_bson_array(const std::vector<T> &values) {
        auto array_builder = bsoncxx::builder::basic::array{};
        for (const auto &v: values) {
            array_builder.append(v);
        }
        return array_builder;
    }

    template<typename T>
    std::vector<T> get_bson_array_to_vector(const bsoncxx::array::view &array) {
        std::vector<T> vec;
        vec.resize(array.length());

        for (const auto &element: array) {
            // 根据类型进行转换
            if constexpr (std::is_same_v<T, int32_t>) {
                vec.push_back(element.get_int32().value);
            }
            else if constexpr (std::is_same_v<T, int64_t>) {
                vec.push_back(element.get_int64().value);
            }
            else if constexpr (std::is_same_v<T, double>) {
                vec.push_back(element.get_double().value);
            }
            else if constexpr (std::is_same_v<T, std::string>) {
                vec.push_back(std::string{element.get_value().get_string().value});
            }
            else {
                throw std::runtime_error("Unsupported type");
            }
        }
        return vec;
    }

}
