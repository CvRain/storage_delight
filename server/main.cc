#include <drogon/drogon.h>
#include <fstream>
#include <nlohmann/json.hpp>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/daily_file_sink.h>

#include "service/group_service.hpp"
#include "service/log_service.hpp"
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

    // 全局异常处理
    drogon::app().setExceptionHandler([](const std::exception &e, const drogon::HttpRequestPtr &req,
                                         std::function<void(drogon::HttpResponsePtr &)> &&callback) {
        LOG_DEBUG << e.what();
        Json::Value json;
        json["code"] = drogon::k500InternalServerError;
        json["error"] = e.what();
        json["data"] = "";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(json);
        callback(resp);
    });

    // 跨域
    drogon::app()
            .registerPreRoutingAdvice([](const drogon::HttpRequestPtr &req, drogon::FilterCallback &&stop,
                                         drogon::FilterChainCallback &&pass) {
                if (req->method() == drogon::HttpMethod::Options) {
                    const auto resp = drogon::HttpResponse::newHttpResponse();
                    resp->addHeader("Access-Control-Allow-Origin", "*");
                    resp->addHeader("Access-Control-Allow-Methods", "GET, POST, PATCH, DELETE, PUT, OPTIONS");
                    stop(resp);
                }
                else {
                    pass();
                }
            })
            .registerSyncAdvice([](const drogon::HttpRequestPtr &req) -> drogon::HttpResponsePtr {
                if (req->method() == drogon::HttpMethod::Options) {
                    auto resp = drogon::HttpResponse::newHttpResponse();
                    {
                        if (const auto &val = req->getHeader("Origin"); !val.empty())
                            resp->addHeader("Access-Control-Allow-Origin", val);
                    }
                    {
                        if (const auto &val = req->getHeader("Access-Control-Request-Method"); !val.empty())
                            resp->addHeader("Access-Control-Allow-Methods", val);
                    }
                    resp->addHeader("Access-Control-Allow-Credentials", "true");
                    {
                        if (const auto &val = req->getHeader("Access-Control-Request-Headers"); !val.empty())
                            resp->addHeader("Access-Control-Allow-Headers", val);
                    }
                    return std::move(resp);
                }
                return {};
            })
            .registerPostHandlingAdvice(
                    [](const drogon::HttpRequestPtr &req, const drogon::HttpResponsePtr &resp) -> void {
                        {
                            if (const auto &val = req->getHeader("Origin"); !val.empty())
                                resp->addHeader("Access-Control-Allow-Origin", val);
                        }
                        {
                            if (const auto &val = req->getHeader("Access-Control-Request-Method"); !val.empty())
                                resp->addHeader("Access-Control-Allow-Methods", val);
                        }
                        resp->addHeader("Access-Control-Allow-Credentials", "true");
                        {
                            if (const auto &val = req->getHeader("Access-Control-Request-Headers"); !val.empty())
                                resp->addHeader("Access-Control-Allow-Headers", val);
                        }
                    })
            .run();

    return 0;
}

void service_init(const nlohmann::json &setting) {
    service_delight::MongoProvider::get_instance().init(setting);
    service_delight::UserService::get_instance().init();
    service_delight::GroupService::get_instance().init();
    service_delight::LogService::get_instance().init();
}