//
// Created by cvrain on 24-9-22.
// 使用spdlog完成的单例模式日志管理类
//

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include "utils/singleton_prototype.hpp"

#include <fmt/format.h>

#include "db_schema.hpp"
#include "type.hpp"

namespace service_delight {
    enum LogOutput : int { ConsoleLogger = 1, BasicLogger, DailyLogger };

    class Logger final : public util_delight::Singleton<Logger> {
    public:
        ~Logger() = default;

        void init();

        [[nodiscard]] std::shared_ptr<spdlog::logger> get_basic_logger() const;

        [[nodiscard]] std::shared_ptr<spdlog::logger> get_daily_logger() const;

        [[nodiscard]] std::shared_ptr<spdlog::logger> get_console_logger() const;

        template<typename... Args>
        void log(int log_output, fmt::format_string<Args...> fmt, Args &&...args);

        template<typename... Args>
        void log(int log_output, spdlog::level::level_enum level, fmt::format_string<Args...> fmt, Args &&...args);

    private:
        std::shared_ptr<spdlog::logger> basic_logger;
        std::shared_ptr<spdlog::logger> daily_logger;
        std::shared_ptr<spdlog::logger> console_logger;
    };

    template<typename... Args>
    void Logger::log(const int log_output, fmt::format_string<Args...> fmt, Args &&...args) {
        // log_output can be: ConsoleLogger, BasicLogger, DailyLogger
        // or ConsoleLogger | BasicLogger | DailyLogger etc..
        // 判断是否需要输出到控制台
        if (log_output & ConsoleLogger) {
            console_logger->info(fmt, std::forward<Args>(args)...);
            console_logger->flush();
        }

        // 判断是否需要输出到基本日志文件
        if (log_output & BasicLogger) {
            basic_logger->info(fmt, std::forward<Args>(args)...);
            basic_logger->flush();
        }

        // 判断是否需要输出到每日日志文件
        if (log_output & DailyLogger) {
            daily_logger->info(fmt, std::forward<Args>(args)...);
            daily_logger->flush();
        }
    }

    template<typename... Args>
    void Logger::log(const int log_output, spdlog::level::level_enum level, fmt::format_string<Args...> fmt,
                     Args &&...args) {
        if (log_output & ConsoleLogger) {
            console_logger->log(level, fmt, std::forward<Args>(args)...);
            console_logger->flush();
        }

        if (log_output & BasicLogger) {
            basic_logger->log(level, fmt, std::forward<Args>(args)...);
            basic_logger->flush();
        }

        if (log_output & DailyLogger) {
            daily_logger->log(level, fmt, std::forward<Args>(args)...);
            daily_logger->flush();
        }
    }
} // service_delight

#endif //LOGGER_HPP
