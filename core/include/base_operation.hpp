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
        auto executeOperation(Func &&operation, const std::string_view &operationName, Args &&...args)
        -> decltype(operation(std::forward<Args>(args)...)) {
            const auto result = operation(std::forward<Args>(args)...);

            if (!result) {
                log(spdlog::level::err, fmt::format("{} operation failed", operationName));
                return result;
            }

            log(spdlog::level::info, fmt::format("{} operation successful", operationName));
            return result;
        }

        void setEnableLogOutputToConsole(bool enable);

        void setEnableLogOutputToFile(bool enable);

    private:
        bool enableLogOutputToConsole = false;
        bool enableLogOutputToFile = false;
    };
}

#endif //STORAGE_DELIGHT_BASE_OPERATION_HPP
