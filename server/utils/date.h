//
// Created by cvrain on 24-8-14.
//

#ifndef STORAGE_DELIGHT_DATE_H
#define STORAGE_DELIGHT_DATE_H

#include <string>
#include <chrono>

namespace util_delight {
    class Date {
    public:
        static inline int32_t get_current_timestamp_32();

        static inline int64_t get_current_timestamp_64();
    };

    int32_t Date::get_current_timestamp_32() {
        return static_cast<int32_t>(std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now().time_since_epoch()).count());
    }

    int64_t Date::get_current_timestamp_64() {
        return std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
    }
}


#endif //STORAGE_DELIGHT_DATE_H
