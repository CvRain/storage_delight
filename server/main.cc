#include <drogon/drogon.h>
#include <fstream>
#include <nlohmann/json.hpp>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/spdlog.h>

#include "models/db_schema.hpp"
#include "service/group_service.hpp"
#include "service/logger.hpp"
#include "service/mongo_service.hpp"
#include "service/user_service.hpp"
#include "utils/string.hpp"

//    auto provider = core::Client::make_provider("uiJ2kXR4V1ceWJPkHNfT", "7KBobqxCWyLQKhARhs6paIsmI4rwx1kx8Zpjghhd");
//    minio::s3::BaseUrl url{"http://server.cloudvl.cn:10569", false}

void service_init(const nlohmann::json &setting);

int main() {
    service_delight::Logger::get_instance().init();
    service_delight::Logger::get_instance().log(service_delight::BasicLogger | service_delight::ConsoleLogger |
                                                        service_delight::DailyLogger,
                                                "Hello storage delight");

    // 初始化访问密钥
    util_delight::StringEncryption::secret_string = util_delight::StringEncryption::generate_secret();
    service_delight::Logger::get_instance().log(service_delight::ConsoleLogger | service_delight::BasicLogger,
                                                "Secret key: {}", util_delight::StringEncryption::secret_string);

    // 初始化配置文件
    const std::string setting_path = "config.json";
    std::cout << &setting_path << std::endl;
    if (!std::filesystem::exists(setting_path)) {
        service_delight::Logger::get_instance().log(service_delight::ConsoleLogger, spdlog::level::err,
                                                    "Config file not found: {}", setting_path);
        return 1;
    }
    // 读取【config.json】配置文件
    std::ifstream config_file(setting_path);

    if (!config_file.is_open()) {
        service_delight::Logger::get_instance().log(service_delight::ConsoleLogger, spdlog::level::err,
                                                    "Failed to open config file: {}", setting_path);
        return 1;
    }
    nlohmann::json config_json;
    config_file >> config_json;

    service_init(config_json);

    if (!config_json.contains("listeners")) {
        service_delight::Logger::get_instance().log(spdlog::level::err, "No listeners found in config file",
                                                    service_delight::BasicLogger | service_delight::ConsoleLogger |
                                                            service_delight::DailyLogger);
    }
    for (const auto &listener: config_json["listeners"]) {
        std::string address = listener.value("address", "0.0.0.0");
        int port = listener.value("port", 80);
        const bool https = listener.value("https", false);

        service_delight::Logger::get_instance().log(
                service_delight::BasicLogger | service_delight::ConsoleLogger | service_delight::DailyLogger,
                "Listening on address: {}, port: {}, {}", address, port, https ? "HTTPS" : "HTTP");
    }


    // Set HTTP listener address and port
    drogon::app().loadConfigFile(setting_path);

    drogon::app().run();


    return 0;
}

void service_init(const nlohmann::json &setting) {
    service_delight::MongoService::get_instance().init(setting);
    service_delight::UserService::get_instance().init();
    service_delight::GroupService::get_instance().init();
}
