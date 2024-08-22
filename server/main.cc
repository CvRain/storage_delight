#include <drogon/drogon.h>
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>
#include <fstream>

#include "service/sqlite_service.h"
#include "utils/string.hpp"

int main() {
    spdlog::info("Hello storage delight server!");
//    auto provider = core::Client::make_provider("uiJ2kXR4V1ceWJPkHNfT", "7KBobqxCWyLQKhARhs6paIsmI4rwx1kx8Zpjghhd");
//    minio::s3::BaseUrl url{"http://server.cloudvl.cn:10569", false}

    //初始化配置文件
    const std::string setting_path = "config.json";

    //初始化访问密钥
    util_delight::StringEncryption::secret_string = util_delight::StringEncryption::generate_secret();
    spdlog::info("secret_string: {}", util_delight::StringEncryption::secret_string);

    //初始化数据库
    spdlog::info("Initialize database");
    SqliteServiceProvider::get_instance().get_sqlite_service();

    //读取【config.json】配置文件
    std::ifstream config_file(setting_path);
    if (!config_file.is_open()) {
        spdlog::error("Failed to open config file: {}", setting_path);
        return 1;
    }
    nlohmann::json config_json;
    config_file >> config_json;

    if (!config_json.contains("listeners")) {
        spdlog::error("Failed to find listeners in config file: {}", setting_path);
    }
    for (const auto &listener: config_json["listeners"]) {
        std::string address = listener.value("address", "0.0.0.0");
        int port = listener.value("port", 80);
        const bool https = listener.value("https", false);
        spdlog::info("Listening on address: {}, port: {}, {}", address, port, https ? "HTTPS" : "HTTP");
    }


    //Set HTTP listener address and port
    drogon::app().loadConfigFile(setting_path);

    drogon::app().run();


    return 0;
}
