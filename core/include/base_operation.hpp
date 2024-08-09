//
// Created by cvrain on 24-7-25.
//

#ifndef STORAGE_DELIGHT_BASE_OPERATION_HPP
#define STORAGE_DELIGHT_BASE_OPERATION_HPP

#include <spdlog/spdlog.h>

namespace storage_delight::core {
    class BaseOperation {
    public:
        void log(spdlog::level::level_enum level, const std::string_view &message) const;

        template<typename Func, typename... Args>
        auto execute_operation(Func &&operation, const std::string_view &operation_name, Args &&...args)
        -> decltype(operation(std::forward<Args>(args)...)) {
            const auto result = operation(std::forward<Args>(args)...);

            if (!result) {
                log(spdlog::level::err, fmt::format("{} operation failed", operation_name));
                return result;
            }

            log(spdlog::level::info, fmt::format("{} operation successful", operation_name));
            return result;
        }

        template<typename Func, typename... Args>
        auto execute_operation(Func &&operation, std::function<std::string_view (void)> operationFunc, Args &&...args)
        -> decltype(operation(std::forward<Args>(args)...)) {
            const auto result = operation(std::forward<Args>(args)...);

            if (!result) {
                log(spdlog::level::err, fmt::format("{}", operationFunc()));
                return result;
            }

            log(spdlog::level::info, fmt::format("{}", operationFunc()));
            return result;
        }

        void set_enable_log_to_console(bool enable);

        void set_enable_log_file(bool enable);

    private:
        bool enable_log_to_console = false;
        bool enable_log_to_file = false;
    };
}

#endif //STORAGE_DELIGHT_BASE_OPERATION_HPP
