//
// Created by cvrain on 24-9-22.
//

#ifndef SINGLETON_PROTOTYPE_HPP
#define SINGLETON_PROTOTYPE_HPP

#include <nlohmann/json.hpp>

namespace util_delight {
    template<typename T>
    class Singleton {
    public:
        static T& get_instance() {
            static T instance;
            return instance;
        }

        Singleton(const Singleton &) = delete;

        Singleton& operator=(const Singleton &) = delete;
    protected:
        Singleton() = default;

        ~Singleton() = default;
    };
}

#endif //SINGLETON_PROTOTYPE_HPP
