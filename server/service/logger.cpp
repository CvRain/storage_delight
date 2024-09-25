//
// Created by cvrain on 24-9-22.
//

#include "logger.hpp"

#include <filesystem>
#include "utils/date.h"

namespace service_delight {
    std::shared_ptr<spdlog::logger> Logger::get_basic_logger() const {
        return basic_logger;
    }

    std::shared_ptr<spdlog::logger> Logger::get_daily_logger() const {
        return daily_logger;
    }

    std::shared_ptr<spdlog::logger> Logger::get_console_logger() const {
        return console_logger;
    }

    void Logger::init() {
        spdlog::set_level(spdlog::level::debug);
        spdlog::set_pattern("[%H:%M:%S %z] [%^--%l--%$] [%n] [thread %t] %v");

        //检查logs目录是否存在，没有则创建
        if (!std::filesystem::exists("logs")) {
            std::filesystem::create_directory("logs");
        }
        try {
            const auto timestamp = std::to_string(util_delight::Date::get_current_timestamp_32());
            basic_logger = spdlog::basic_logger_mt("basic_logger", "logs/server-basic-" + timestamp + ".log");
            daily_logger = spdlog::daily_logger_mt("daily_logger", "logs/server-daily.log", 0, 0);
        }
        catch (const spdlog::spdlog_ex &ex) {
            spdlog::error("Log initialization failed: {0}", ex.what());
        }
        basic_logger->set_level(spdlog::level::debug);
        daily_logger->set_level(spdlog::level::debug);

        console_logger = spdlog::stdout_color_mt("console_logger");
        console_logger->set_level(spdlog::level::debug);

        console_logger->info("logger init success");
    }
} // service_delight
